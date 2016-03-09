/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_FUTURE_HPP
#define STLAB_FUTURE_HPP

#include <future>
#include <initializer_list>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <boost/optional.hpp>

#include <stlab/config.hpp>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN
#include <emscripten.h>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <typename...> struct type_list { };

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> { using type = R; };

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

template <typename>
struct arguments_of_;

template <typename R, typename... Args>
struct arguments_of_<R(Args...)> { using type = type_list<Args...>; };

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename...> class packaged_task;

template <typename, typename = void> class future;

using schedule_t = std::function<void(std::function<void()>)>;

/**************************************************************************************************/

namespace detail {

template <typename> struct packaged_task_from_signature;

template <typename R, typename... Args>
struct packaged_task_from_signature<R (Args...)> {
    using type = packaged_task<Args...>;
};
template <typename T>
using packaged_task_from_signature_t = typename packaged_task_from_signature<T>::type;

} // namespace detail

/**************************************************************************************************/

template <typename Sig, typename S, typename F>
auto package(S, F)
    -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>>;

/**************************************************************************************************/

namespace detail {

template <typename> struct shared;
template <typename, typename = void> struct shared_base;

template <typename T>
using enable_if_copyable = std::enable_if_t<std::is_copy_constructible<T>::value>;

template <typename T>
using enable_if_not_copyable = std::enable_if_t<!std::is_copy_constructible<T>::value>;

/**************************************************************************************************/

template <typename T>
struct shared_future {
    virtual ~shared_future() = default;
};

template <typename... Args>
struct shared_task {
    virtual ~shared_task() = default;
    virtual void remove_promise() = 0;
    virtual void add_promise() = 0;

    virtual void operator()(Args... args) = 0;
};

/**************************************************************************************************/

template <typename T>
struct shared_base<T, enable_if_copyable<T>> : std::enable_shared_from_this<shared_base<T>> {
    using then_t = std::vector<std::pair<schedule_t, std::function<void()>>>;

    schedule_t                          _schedule;
    boost::optional<T>                  _result;
    boost::optional<std::exception_ptr> _error;
    std::mutex                          _mutex;
    bool                                _ready = false;
    then_t                              _then;

    explicit shared_base(schedule_t s) : _schedule(std::move(s)) { }

    template <typename F>
    auto then(F f) { return then(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto then(S s, F f) {
        return recover(std::move(s), [_f = std::move(f)](const auto& x){
            return _f(*x.get_try());
        });
    }

    template <typename F>
    auto recover(F f) { return recover(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto recover(S s, F f) {
        auto p = package<std::result_of_t<F(future<T>)>()>(s,
            [_f = std::move(f), _p = future<T>(this->shared_from_this())] {
                return _f(_p);
            });

        bool ready;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) 
                _then.emplace_back(std::move(s), std::move(p.first));
        }
        if (ready) 
            s(std::move(p.first));

        return std::move(p.second);
    }

    template <typename F>
    auto then_r(bool unique, F f) { return then_r(unique, _schedule, std::move(f)); }

    template <typename S, typename F>
    auto then_r(bool unique, S s, F f) {
        return recover_r(unique, std::move(s), [_f = std::move(f)](auto x){
            return _f(*std::move(x).get_try());
        });
    }

    template <typename F>
    auto recover_r(bool unique, F f) { return recover_r(unique, _schedule, std::move(f)); }

    /*
        REVISIT (sparent) : Need to write test cases for all the r-value cases.
        The logic here is that if this is the only reference and it is an rvalue than this
        is the last recover clause added and so it can consume the value.
    */

    template <typename S, typename F>
    auto recover_r(bool unique, S s, F f) {
        if (!unique) return recover(std::move(s),std::move(f));

        auto p = package<std::result_of_t<F(future<T>)>()>(s,
            [_f = std::move(f), _p = future<T>(this->shared_from_this())] {
                return _f(std::move(_p));
            });

        bool ready;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) 
                _then.emplace_back(std::move(s), std::move(p.first));
        }
        if (ready) s(std::move(p.first));

        return std::move(p.second);
    }

    void set_exception(std::exception_ptr error) {
        _error = std::move(error);
        then_t then;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            then = move(_then);
            _ready = true;
        }
        // propogate exception without scheduling
        for (const auto& e : then) e.second();
    }

    template <typename F, typename... Args>
    void set_value(const F& f, Args&&... args);

    auto get_try() -> boost::optional<T> {
        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_error) std::rethrow_exception(_error.get());
            return _result;
        }
        return boost::none;
    }

    auto get_try_r(bool unique) -> boost::optional<T> {
        if (!unique) return get_try();

        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_error) std::rethrow_exception(_error.get());
            return std::move(_result);
        }
        return boost::none;
    }
};

/**************************************************************************************************/

template <typename T>
struct shared_base<T, enable_if_not_copyable<T>> : std::enable_shared_from_this<shared_base<T>> {
    using then_t = std::pair<schedule_t, std::function<void()>>;

    schedule_t                          _schedule;
    boost::optional<T>                  _result;
    boost::optional<std::exception_ptr> _error;
    std::mutex                          _mutex;
    bool                                _ready = false;
    then_t                              _then;

    explicit shared_base(schedule_t s) : _schedule(std::move(s)) { }

    template <typename F>
    auto then_r(bool unique, F f) { return then_r(unique, _schedule, std::move(f)); }

    template <typename S, typename F>
    auto then_r(bool unique, S s, F f) {
        return recover_r(unique, std::move(s), [_f = std::move(f)](auto x){
            return _f(*std::move(x).get_try());
        });
    }

    template <typename F>
    auto recover_r(bool unique, F f) { return recover_r(unique, _schedule, std::move(f)); }

    template <typename S, typename F>
    auto recover_r(bool, S s, F f) {
        // rvalue case unique is assumed.
        auto p = package<std::result_of_t<F(future<T>)>()>(s,
            [_f = std::move(f), _p = future<T>(this->shared_from_this())] {
                return _f(std::move(_p));
            });

        bool ready;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) 
                _then = { std::move(s), std::move(p.first) };
        }
        if (ready) 
            s(std::move(p.first));

        return std::move(p.second);
    }

    void set_exception(std::exception_ptr error) {
        _error = std::move(error);
        then_t then;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            then = std::move(_then);
            _ready = true;
        }
        // propogate exception without scheduling
        then.second();
    }

    template <typename F, typename... Args>
    void set_value(const F& f, Args&&... args);

    auto get_try() -> boost::optional<T> {
        return get_try_r(true);
    }

    auto get_try_r(bool) -> boost::optional<T> {
        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_error) std::rethrow_exception(_error.get());
            return std::move(_result);
        }
        return boost::none;
    }
};

/**************************************************************************************************/

template <>
struct shared_base<void> : std::enable_shared_from_this<shared_base<void>> {
    using then_t = std::vector<std::pair<schedule_t, std::function<void()>>>;

    schedule_t                          _schedule;
    boost::optional<std::exception_ptr> _error;
    std::mutex                          _mutex;
    bool                                _ready = false;
    then_t                              _then;

    explicit shared_base(schedule_t s) : _schedule(std::move(s)) { }

    template <typename F>
    auto then(F f) { return then(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto then(S s, F f) {
        return recover(std::move(s), [_f = std::move(f)](auto x){
            x.get_try(); // throw if error
            return _f();
        });
    }

    template <typename F>
    auto then_r(bool, F f) { return then(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto then_r(bool, S s, F f) { return then(std::move(s), std::move(f)); }

    template <typename F>
    auto recover(F f) { return recover(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto recover(S s, F f) -> future<std::result_of_t<F(future<void>)>>;

    template <typename F>
    auto recover_r(bool, F f) { return recover(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto recover_r(bool, S s, F f) { return recover(std::move(s), std::move(f)); }

    void set_exception(std::exception_ptr error) {
        _error = std::move(error);
        then_t then;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            then = std::move(_then);
            _ready = true;
        }
        // propogate exception without scheduling
        for (const auto& e : then) e.second();
    }

    auto get_try() -> bool {
        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_error) std::rethrow_exception(_error.get());
            return true;
        }
        return false;
    }

    auto get_try_r(bool) { return get_try(); }

    template <typename F, typename... Args>
    void set_value(const F& f, Args&&... args);
};

template <typename R, typename... Args>
struct shared<R (Args...)> : shared_base<R>, shared_task<Args...>
{
    using function_t = std::function<R (Args...)>;

    std::atomic_size_t _promise_count;
    function_t _f;

    template <typename F>
    explicit shared(schedule_t s, F f) : shared_base<R>(std::move(s)), _f(std::move(f)) {
        _promise_count = 1;
    }

    void remove_promise() override {
        if (--_promise_count == 0) {
            std::unique_lock<std::mutex> lock(this->_mutex);
            if (!this->_ready) {
                _f = function_t();
                this->_error = std::make_exception_ptr(std::future_error(
                    std::future_errc::broken_promise));
                this->_ready = true;
            }
        }
    }
    void add_promise() override { ++_promise_count; }

    void operator()(Args... args) override {
        if (_f) try {
            this->set_value(_f, std::move(args)...);
        } catch(...) {
            this->set_exception(std::current_exception());
        }
        _f = function_t();
    }
};

/**************************************************************************************************/

} // detail

/**************************************************************************************************/

template<typename... Args>
class packaged_task {
    using ptr_t = std::weak_ptr<detail::shared_task<Args...>>;

    ptr_t _p;

    explicit packaged_task(ptr_t p) : _p(std::move(p)) { }

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<detail::packaged_task_from_signature_t<Signature>,
                future<detail::result_of_t_<Signature>>>;
public:
    packaged_task() = default;

    ~packaged_task() {
        auto p = _p.lock();
        if (p) p->remove_promise();
    }

    packaged_task(const packaged_task& x) : _p(x._p) {
        auto p = _p.lock();
        if (p) p->add_promise();
    }

    packaged_task(packaged_task&&) noexcept = default;
    packaged_task& operator=(const packaged_task& x) {
        auto tmp = x; *this = std::move(tmp); return *this;
    }
    packaged_task& operator=(packaged_task&& x) noexcept = default;

    template <typename... A>
    void operator()(A&&... args) const {
        auto p = _p.lock();
        if (p) (*p)(std::forward<A>(args)...);
    }
};

/**************************************************************************************************/

template <typename T>
class future<T, detail::enable_if_copyable<T>> {
    using ptr_t = std::shared_ptr<detail::shared_base<T>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) { }

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<detail::packaged_task_from_signature_t<Signature>,
                future<detail::result_of_t_<Signature>>>;

    friend struct detail::shared_base<T>;

  public:
    using result_type = T;

    future() = default;

    bool valid() const { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) const& {
        assert(_p);
        return _p->then(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) const& {
        assert(_p);
        return _p->then(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto then(F&& f) && {
        assert(_p);
        return _p->then_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        assert(_p);
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) const& {
        assert(_p);
        return _p->recover(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) const& {
        assert(_p);
        return _p->recover(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) && {
        assert(_p);
        return _p->recover_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) && {
        assert(_p);
        return _p->recover_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    void detach() const {
        assert(_p); 
        then([_hold = _p](auto f){ }, [](const auto& x){ });
    }

    bool cancel_try() {
        if (!_p.unique()) return false;
        std::weak_ptr<detail::shared_base<T>> p = _p;
        _p.reset();
        _p = p.lock();
        return !_p;
    }

    auto get_try() const& {
        assert(_p); 
        return _p->get_try();
    }

    auto get_try() && {
        assert(_p); 
        return _p->get_try_r(_p.unique());
    }

    boost::optional<std::exception_ptr> error() const {
        assert(_p);
        return _p->_error;
    }
};

/**************************************************************************************************/

template <>
class future<void, void> {
    using ptr_t = std::shared_ptr<detail::shared_base<void>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) { }

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<detail::packaged_task_from_signature_t<Signature>,
                future<detail::result_of_t_<Signature>>>;
                
    friend struct detail::shared_base<void>;

  public:
    using result_type = void;

    future() = default;

    bool valid() const { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) const& {
        assert(_p); 
        return _p->then(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) const& {
        assert(_p);
        return _p->then(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto then(F&& f) && {
        assert(_p);
        return _p->then_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        assert(_p);
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) const& {
        assert(_p); 
        return _p->recover(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) const& {
        assert(_p);
        return _p->recover(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) && {
        assert(_p);
        return _p->recover_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) && {
        assert(_p);
        return _p->recover_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    void detach() const {
        assert(_p); 
        then([_hold = _p](auto f){ }, [](){ });
    }

    bool cancel_try() {
        if (!_p.unique()) return false;
        std::weak_ptr<detail::shared_base<void>> p = _p;
        _p.reset();
        _p = p.lock();
        return !_p;
    }

    bool get_try() {
        assert(_p);
        return _p->get_try();
    }

    boost::optional<std::exception_ptr> error() const {
        assert(_p);
        return _p->_error;
    }

};

/**************************************************************************************************/

template <typename T>
class future<T, detail::enable_if_not_copyable<T>> {
    using ptr_t = std::shared_ptr<detail::shared_base<T>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) { }

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<detail::packaged_task_from_signature_t<Signature>,
                future<detail::result_of_t_<Signature>>>;
    
    friend struct detail::shared_base<T>;

  public:
    using result_type = T;

    future() = default;
    future(const future&) = delete;
    future(future&&) noexcept = default;
    future& operator=(const future&) = delete;
    future& operator=(future&&) noexcept = default;

    bool valid() const { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) && {
        assert(_p);
        return _p->then_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        assert(_p);
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) && {
        assert(_p);
        return _p->recover_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) && {
        assert(_p);
        return _p->recover_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    void detach() const {
        assert(_p); 
        then([_hold = _p](auto f){ }, [](const auto& x){ });
    }

    bool cancel_try() {
        if (!_p.unique()) return false;
        std::weak_ptr<detail::shared_base<T>> p = _p;
        _p.reset();
        _p = p.lock();
        return !_p;
    }

    auto get_try() const& {
        assert(_p); 
        return _p->get_try();
    }

    auto get_try() && {
        assert(_p);
        return _p->get_try_r(_p.unique());
    }

    boost::optional<std::exception_ptr> error() const {
        assert(_p);
        return _p->_error;
    }
};

template <typename Sig, typename S, typename F>
auto package(S s, F f) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>> {
    auto p = std::make_shared<detail::shared<Sig>>(std::move(s), std::move(f));
    return std::make_pair(detail::packaged_task_from_signature_t<Sig>(p),
            future<detail::result_of_t_<Sig>>(p));
}

/**************************************************************************************************/

namespace detail {

template <typename F, typename... Ts>
struct when_all_shared {
    // decay
    std::tuple<boost::optional<Ts>...>  _args;
    future<void>                        _holds[sizeof...(Ts)] {};
    std::atomic_size_t                  _remaining {sizeof...(Ts)};
    std::atomic_bool                    _error_happend{ false };
    std::mutex                          _errormutex;
    boost::optional<std::exception_ptr> _error;
    packaged_task<>                     _f;

    void done() { if (--_remaining == 0 && !_error_happend) _f(); }

    void failure(std::exception_ptr error) {
        {
            std::unique_lock<std::mutex> lock(_errormutex);
            _error = std::move(error);
            _error_happend = true;
        }
        for (auto& h : _holds) {
            h.cancel_try();
        }
        _f();
    }

};

inline void throw_if_false(bool x, boost::optional<std::exception_ptr>& p) {
    if (!x) std::rethrow_exception(p.get());;
}

template <typename F, typename Args, typename P, std::size_t... I>
auto apply_when_all_args_(const F& f, Args& args, P& p, std::index_sequence<I...>) {
    (void)std::initializer_list<int>{(throw_if_false(std::get<I>(args).is_initialized(), p->_error), 0)... };
    return f(std::move(std::get<I>(args).get())...);
}

template <typename F, typename P>
auto apply_when_all_args(const F& f, P& p) {
    return apply_when_all_args_(f, p->_args, p, std::make_index_sequence<std::tuple_size<decltype(p->_args)>::value>());
}

template <std::size_t i, typename P, typename T>
void attach_when_all_arg_(const std::shared_ptr<P>& p, T a) {
    p->_holds[i] = std::move(a).recover([_w = std::weak_ptr<P>(p)](auto x){
        auto p = _w.lock(); if (!p) return;

        auto error = x.error();
        if (error) {
            p->failure(*error);
        }
        else {
            std::get<i>(p->_args) = *std::move(x).get_try();
            p->done();
        }
    });
}

template <typename P, typename... Ts, std::size_t... I>
void attach_when_all_args_(std::index_sequence<I...>, const std::shared_ptr<P>& p, Ts... a) {
    (void)std::initializer_list<int>{(attach_when_all_arg_<I>(p, a), 0)...};
}

template <typename P, typename... Ts>
void attach_when_all_args(const std::shared_ptr<P>& p, Ts... a) {
    attach_when_all_args_(std::make_index_sequence<sizeof...(Ts)>(), p, std::move(a)...);
}

} // namespace detail

/**************************************************************************************************/

template <typename S, typename F, typename... Ts>
auto when_all(S s, F f, future<Ts>... args) {
    using result_t = typename std::result_of<F(Ts...)>::type;

    auto shared = std::make_shared<detail::when_all_shared<F, Ts...>>();
    auto p = package<result_t()>(std::move(s), [_f = std::move(f), _p = shared] {
        return detail::apply_when_all_args(_f, _p);
    });
    shared->_f = std::move(p.first);

    detail::attach_when_all_args(shared, std::move(args)...);

    return std::move(p.second);
}

/**************************************************************************************************/

namespace detail
{
    template <typename F, typename I, typename Input>
    struct when_all_context {
      when_all_context(I first, I last)
        : _remaining(std::distance(first, last))
        , _results(_remaining)
        , _holds(_remaining)
      {}

      std::atomic_size_t                    _remaining;
      std::atomic_bool                      _error_happened{false};
      std::vector<Input>                    _results;
      std::vector<future<void>>             _holds;
      std::mutex                            _errormutex;
      boost::optional<std::exception_ptr>   _error;
      packaged_task<>                       _f;

      void done() { if (--_remaining == 0 && !_error_happened) _f(); }

      void failure(std::exception_ptr error) {
          {
              std::unique_lock<std::mutex> lock(_errormutex);
              _error = std::move(error);
              _error_happened = true;
          }
          for (auto& h : _holds) {
              h.cancel_try();
          }
          _f();
      }
    };

    template <typename F, typename I>
    struct when_all_context<F, I, void> {
        when_all_context(I first, I last)
            : _remaining(std::distance(first, last))
            , _holds(_remaining)
        {}

        std::atomic_size_t                    _remaining;
        std::atomic_bool                      _error_happened{ false };
        std::vector<future<void>>             _holds;
        std::mutex                            _errormutex;
        boost::optional<std::exception_ptr>   _error;
        packaged_task<>                       _f;

        void done() { if (--_remaining == 0 && !_error_happened) _f(); }

        void failure(std::exception_ptr error) {
            {
                std::unique_lock<std::mutex> lock(_errormutex);
                _error = std::move(error);
                _error_happened = true;
            }
            for (auto& h : _holds) {
                h.cancel_try();
            }
            _f();
        }
    };

    template <typename P, typename T>
    void attach_when_all_tasks(size_t index, const std::shared_ptr<P>& p, T a) {
        p->_holds[index] = std::move(a).recover([_w = std::weak_ptr<P>(p), _i = index](auto x){
            auto p = _w.lock(); if (!p) return;
            auto error = x.error();
            if (error) {
                p->failure(*error);
            }
            else {
                p->_results[_i] = std::move(*x.get_try());
                p->done();
            }
        });
    }

    template <typename P, typename T>
    void attach_when_all_void_tasks(size_t index, const std::shared_ptr<P>& p, T a) {
        p->_holds[index] = std::move(a).recover([_w = std::weak_ptr<P>(p)](auto x){
            auto p = _w.lock(); if (!p) return;
            auto error = x.error();
            if (error) {
                p->failure(*error);
            }
            else {
                p->done();
            }
        });
    }

    template <typename R>
    struct when_all_base_collector;

    template <typename R>
    struct when_all_base_collector {

        template<typename S, typename F, typename I>
        static auto collect(S&& s, F&& f, I first, I last) {
            using result_t = typename std::result_of<F(std::vector<R>)>::type;

            if (first == last) {
                auto p = package<result_t()>(std::forward<S>(s), std::bind(std::forward<F>(f), std::vector<R>()));
                s(std::move(p.first));
                return std::move(p.second);
            }

            auto context = std::make_shared<detail::when_all_context<F, I, R>>(first, last);
            auto p = package<result_t()>(std::move(s), [_f = std::move(f), _c = context] {
                if (_c->_error) {
                    std::rethrow_exception(_c->_error.get());
                }
                return _f(_c->_results);
            });

            context->_f = std::move(p.first);

            size_t index(0);
            std::for_each(first, last, [&index, &context](auto item) {
                detail::attach_when_all_tasks(index++, context, item);
            });

            return std::move(p.second);

        }
    };

    template <>
    struct when_all_base_collector<void> {

        template<typename S, typename F, typename I>
        static auto collect(S&& s, F&& f, I first, I last) {
            using result_t = typename std::result_of<F()>::type;

            if (first == last) {
                auto p = package<void()>(std::forward<S>(s), std::forward<F>(f));
                s(std::move(p.first));
                return std::move(p.second);
            }

            auto context = std::make_shared<detail::when_all_context<F, I, void>>(first, last);
            auto p = package<result_t()>(std::move(s), [_f = std::move(f), _c = context]{
                if (_c->_error) {
                    std::rethrow_exception(_c->_error.get());
                }
                return _f();
            });

            context->_f = std::move(p.first);

            size_t index(0);
            std::for_each(first, last, [&index, &context](auto item) {
                detail::attach_when_all_void_tasks(index++, context, item);
            });

            return std::move(p.second);

        }
    };
}


/**************************************************************************************************/

template <typename S, // models task scheduler
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures
auto when_all(S schedule, F f, const std::pair<I, I>& range) {
    using param_t = typename std::iterator_traits<I>::value_type::result_type;

    return detail::when_all_base_collector<param_t>::collect(std::forward<S>(schedule), std::forward<F>(f), range.first, range.second);
}

/**************************************************************************************************/

template <typename S, typename F, typename ...Args>
auto async(S schedule, F&& f, Args&&... args)
        -> future<std::result_of_t<F (Args...)>>
{
    auto p = package<std::result_of_t<F(Args...)>()>(schedule,
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    schedule(std::move(p.first));
    
    return std::move(p.second);
}

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <typename F, typename... Args>
void shared_base<void>::set_value(const F& f, Args&&... args) {
    f(std::forward<Args>(args)...);
    then_t then;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
        then = std::move(_then);
    }
    for (const auto& e : then) e.first(e.second);
}


template <typename S, typename F>
auto shared_base<void>::recover(S s, F f) -> future<std::result_of_t<F(future<void>)>>
 {
    auto p = package<std::result_of_t<F(future<void>)>()>(s,
        [_f = std::move(f), _p = future<void>(this->shared_from_this())] {
            return _f(_p);
        });

    bool ready;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        ready = _ready;
        if (!ready) 
            _then.emplace_back(std::move(s), std::move(p.first));
    }
    if (ready) 
        s(std::move(p.first));

    return std::move(p.second);
}

template <typename T>
template <typename F, typename... Args>
void shared_base<T, enable_if_copyable<T>>::set_value(const F& f, Args&&... args) {
    _result = f(std::forward<Args>(args)...);
    then_t then;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
        then = std::move(_then);
    }
    for (const auto& e : then) e.first(e.second);
}

template <typename T>
template <typename F, typename... Args>
void shared_base<T, enable_if_not_copyable<T>>::set_value(const F& f, Args&&... args) {
    _result = f(std::forward<Args>(args)...);
    then_t then;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
        then = std::move(_then);
    }
    if (then.first) then.first(then.second);
}

#if 0
/*
    REVIST (sparent) : This is doing reduction on future<void> we also need to do the same for
    other result types.
*/

template <>
template <typename F, typename... Args>
void shared_base<future<void>>::set_value(const F& f, Args&&... args) {
    _result = f(std::forward<Args>(args)...).then([_p = this->shared_from_this()]() {
        auto& self = *_p;
        then_t then;
        {
        std::unique_lock<std::mutex> lock(self._mutex);
        self._ready = true;
        then = std::move(self._then);
        }
        for (const auto& e : then) e.first(e.second);
    });
}
#endif

/**************************************************************************************************/

void async_(std::function<void()>);

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

struct default_scheduler {
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        dispatch_async_f(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                new f_t(std::move(f)), [](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                });
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN

struct default_scheduler {
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        // REVISIT (sparent) : Using a negative timeout may give better performance. Need to test.
        using f_t = decltype(f);

        emscripten_async_call([](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                }, new f_t(std::move(f)), 0);
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

struct default_scheduler {
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        detail::async_(std::move(f));
    }
};

#endif

/**************************************************************************************************/

template <typename T>
future<T> make_ready_future(T&& x) {
    auto p = package<T(T)>(default_scheduler(), [](auto&& x) { return x; });
    p.first(x);
    return p.second;
}

inline future<void> make_ready_future() {
    auto p = package<void()>(default_scheduler(), [](){});
    p.first();
    return p.second;
}

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
