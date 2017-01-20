/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_FUTURE_HPP
#define STLAB_FUTURE_HPP

#include <atomic>
#include <future>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <boost/optional.hpp>

#include <stlab/config.hpp>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN
#include <emscripten.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/core.h>
#include <ppapi/cpp/completion_callback.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS
#include <Windows.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE
// REVISIT (sparent) : for testing only
#if 0 && __APPLE__
#include <dispatch/dispatch.h>
#endif
#endif

// usefull makro for debugging
#define STLAB_TRACE(S) \
    printf("%s:%d %d %s\n", __FILE__, __LINE__, (int)std::hash<std::thread::id>()(std::this_thread::get_id()), S);

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


template <typename F, typename T>
struct result_of_when_all_t;

template <typename F>
struct result_of_when_all_t<F,void>
{
    using result_type = typename std::result_of<F()>::type;
};

template <typename F, typename T>
struct result_of_when_all_t
{
    using result_type = typename std::result_of<F(const std::vector<T>&)>::type;
};


template <typename F, typename T>
struct result_of_when_any_t;

template <typename F>
struct result_of_when_any_t<F, void>
{
    using result_type = typename std::result_of<F(size_t)>::type;
};

template <typename F, typename R>
struct result_of_when_any_t
{
    using result_type = typename std::result_of<F(R, size_t)>::type;
};




/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename...> class packaged_task;

template <typename, typename = void> class future;

using schedule_t = std::function<void(std::function<void()>)>;
using timed_schedule_t = std::function<void(std::chrono::system_clock::time_point, std::function<void()>)>;

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

template <bool...> struct bool_pack;
template <bool... v>
using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

/**************************************************************************************************/

template <typename T>
using enable_if_copyable = std::enable_if_t<std::is_copy_constructible<T>::value>;

template <typename T>
using enable_if_not_copyable = std::enable_if_t<!std::is_copy_constructible<T>::value>;

/**************************************************************************************************/

template <typename> struct shared;
template <typename, typename = void> struct shared_base;

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
            return _f(x.get_try().value());
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
            return _f(std::move(x).get_try().get());
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
        // propagate exception without scheduling
        for (const auto& e : then) { e.second(); }
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
            return _f(std::move(x).get_try().value());
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
            if (_then.second)
                then = std::move(_then);
            _ready = true;
        }
        // propagate exception without scheduling
        if (then.second) then.second();
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
        // propagate exception without scheduling 
        for (const auto& e : then) { e.second(); }
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
    shared(schedule_t s, F f) : shared_base<R>(std::move(s)), _f(std::move(f)) {
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

    template <typename Signature, typename S, typename F>
    friend auto package_with_broken_promise(S, F)
        ->std::pair<detail::packaged_task_from_signature_t<Signature>,
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

    template <typename Signature, typename S, typename F>
    friend auto package_with_broken_promise(S, F)
        ->std::pair<detail::packaged_task_from_signature_t<Signature>,
        future<detail::result_of_t_<Signature>>>;

    friend struct detail::shared_base<T>;

  public:
    using result_type = T;

    future() = default;

    bool valid() const { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) const& {
        return _p->then(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) const& {
        return _p->then(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto then(F&& f) && {
        return _p->then_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) const& {
        return _p->recover(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) const& {
        return _p->recover(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) && {
        return _p->recover_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) && {
        return _p->recover_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    void detach() const {
        then([_hold = _p](auto f){ }, [](const auto& x){ });
    }

    void reset() {
        _p.reset();
    }

    auto get_try() const& {
        return _p->get_try();
    }

    // Fp Does it make sense to have this? At the moment I don't see a real use case for it.
    // One can only ask once on an r-value and then the future is gone.
    // To perform this in an l-value casted to an r-value does not make sense either,
    // because in this case _p is not unique any more and internally it is forwarded to
    // the l-value get_try.
    auto get_try() && {
        return _p->get_try_r(_p.unique());
    }

    boost::optional<std::exception_ptr> error() const {
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

    template <typename Signature, typename S, typename F>
    friend auto package_with_broken_promise(S, F)
        ->std::pair<detail::packaged_task_from_signature_t<Signature>,
        future<detail::result_of_t_<Signature>>>;

    friend struct detail::shared_base<void>;

  public:
    using result_type = void;

    future() = default;

    bool valid() const { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) const& {
        return _p->then(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) const& {
        return _p->then(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto then(F&& f) && {
        return _p->then_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) const& {
        return _p->recover(std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) const& {
        return _p->recover(std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) && {
        return _p->recover_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) && {
        return _p->recover_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    void detach() const {
        then([_hold = _p](auto f){ }, [](){ });
    }

    void reset() {
        _p.reset();
    }

    bool get_try() const& {
        return _p->get_try();
    }

    boost::optional<std::exception_ptr> error() const {
        return _p->_error;
    }

};

/**************************************************************************************************/

template <typename T>
class future<T, detail::enable_if_not_copyable<T>> {
    using ptr_t = std::shared_ptr<detail::shared_base<T>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) { }
    future(const future&) = default;

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<detail::packaged_task_from_signature_t<Signature>,
                future<detail::result_of_t_<Signature>>>;

    template <typename Signature, typename S, typename F>
    friend auto package_with_broken_promise(S, F)
        ->std::pair<detail::packaged_task_from_signature_t<Signature>,
        future<detail::result_of_t_<Signature>>>;

    friend struct detail::shared_base<T>;

  public:
    using result_type = T;

    future() = default;
    future(future&&) noexcept = default;
    future& operator=(const future&) = delete;
    future& operator=(future&&) noexcept = default;

    bool valid() const { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) && {
        return _p->then_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    template <typename F>
    auto recover(F&& f) && {
        return _p->recover_r(_p.unique(), std::forward<F>(f));
    }

    template <typename S, typename F>
    auto recover(S&& s, F&& f) && {
        return _p->recover_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    void detach() const {
        _p->then_r(_p.unique(), [_hold = _p](auto f) {}, [](auto&&) {});
    }

    void reset() {
        _p.reset();
    }

    auto get_try() const& {
        return _p->get_try();
    }

    auto get_try() && {
        return _p->get_try_r(_p.unique());
    }

    boost::optional<std::exception_ptr> error() const {
        return _p->_error;
    }
};

template <typename Sig, typename S, typename F>
auto package(S s, F f) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>> {
    auto p = std::make_shared<detail::shared<Sig>>(std::move(s), std::move(f));
    return std::make_pair(detail::packaged_task_from_signature_t<Sig>(p),
            future<detail::result_of_t_<Sig>>(p));
}

template <typename Sig, typename S, typename F>
auto package_with_broken_promise(S s, F f) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>> {
    auto p = std::make_shared<detail::shared<Sig>>(std::move(s), std::move(f));
    auto result = std::make_pair(detail::packaged_task_from_signature_t<Sig>(p),
        future<detail::result_of_t_<Sig>>(p));
    result.second._p->_error = std::make_exception_ptr(std::future_error(std::future_errc::broken_promise));
    result.second._p->_ready = true;
    return result;
}

/**************************************************************************************************/

namespace detail {

template <typename F, typename... Ts>
struct when_all_shared {
    // decay
    std::tuple<boost::optional<Ts>...>  _args;
    future<void>                        _holds[sizeof...(Ts)] {};
    std::atomic_size_t                  _remaining{sizeof...(Ts)};
    std::atomic_flag                    _error_happened = ATOMIC_FLAG_INIT;
    boost::optional<std::exception_ptr> _error;
    packaged_task<>                     _f;

    template <std::size_t index, typename FF>
    void done(FF& f) { 
        std::get<index>(_args) = std::move(f.get_try().value()); 
        if (--_remaining == 0) _f(); 
    }

    void failure(std::exception_ptr error) {
        auto before = _error_happened.test_and_set();
        if (before == false) {
            for (auto& h : _holds) h.reset();
            _error = std::move(error);
            _f();
        }
    }

};

template <typename F, size_t S, typename R>
struct when_any_shared {
    // decay
    boost::optional<R>                  _arg;
    future<void>                        _holds[S]{};
    std::atomic_size_t                  _remaining{S};
    std::atomic_flag                    _value_received = ATOMIC_FLAG_INIT;
    boost::optional<std::exception_ptr> _error;
    size_t                              _index;
    packaged_task<>                     _f;

    void failure(std::exception_ptr error) {
        if (--_remaining == 0) {
            _error = std::move(error);
            _f();
        }
    }

    template <size_t index, typename FF>
    void done(FF&& f) {
        auto before = _value_received.test_and_set();
        if (before == false) {
            _arg = std::move(std::forward<FF>(f).get_try().value());
            _index = index;
            _f();
        }
    }
};

inline void rethrow_if_false(bool x, boost::optional<std::exception_ptr>& p) {
    if (!x) std::rethrow_exception(p.get());;
}

template <typename F, typename Args, typename P, std::size_t... I>
auto apply_when_all_args_(const F& f, Args& args, P& p, std::index_sequence<I...>) {
    (void)std::initializer_list<int>{(rethrow_if_false(std::get<I>(args).is_initialized(), p->_error), 0)... };
    return f(std::move(std::get<I>(args).get())...);
}

template <typename F, typename P>
auto apply_when_all_args(const F& f, P& p) {
    return apply_when_all_args_(f, p->_args, p, std::make_index_sequence<std::tuple_size<decltype(p->_args)>::value>());
}

template <typename F, typename P>
auto apply_when_any_arg(const F& f, P& p) {
    if (p->_error) {
        std::rethrow_exception(p->_error.get());
    }

    return f(std::move(p->_arg.get()), p->_index);
}

template <std::size_t i, typename P, typename T>
void attach_when_arg_(const std::shared_ptr<P>& p, T a) {
    p->_holds[i] = std::move(a).recover([_w = std::weak_ptr<P>(p)](auto x){
        auto p = _w.lock(); if (!p) return;

        auto error = x.error();
        if (error) {
            p->failure(*error);
        }
        else {
            p->template done<i>(x);
        }
    });
}

template <typename P, typename... Ts, std::size_t... I>
void attach_when_args_(std::index_sequence<I...>, const std::shared_ptr<P>& p, Ts... a) {
    (void)std::initializer_list<int>{(attach_when_arg_<I>(p, a), 0)...};
}

template <typename P, typename... Ts>
void attach_when_args(const std::shared_ptr<P>& p, Ts... a) {
    attach_when_args_(std::make_index_sequence<sizeof...(Ts)>(), p, std::move(a)...);
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

    detail::attach_when_args(shared, std::move(args)...);

    return std::move(p.second);
}

/**************************************************************************************************/

template <typename S, typename F, typename T, typename... Ts>
auto when_any(S s, F f, future<T> arg, future<Ts>... args) {
    using result_t = typename std::result_of<F(T, size_t)>::type;

    auto shared = std::make_shared<detail::when_any_shared<F, sizeof...(Ts)+1, T>>();
    auto p = package<result_t()>(std::move(s), [_f = std::move(f), _p = shared]{
        return detail::apply_when_any_arg(_f, _p);
    });
    shared->_f = std::move(p.first);

    detail::attach_when_args(shared, std::move(arg), std::move(args)...);

    return std::move(p.second);
}

/**************************************************************************************************/

namespace detail
{
    template <typename T>
    struct value_storer
    {
        template <typename C, typename F>
        static void store(C& c, F&& f, size_t index) {
            c._results = std::move(*std::forward<F>(f).get_try());
            c._index = index;
        }
    };

    template <typename T>
    struct value_storer<std::vector<T>>
    {
        template <typename C, typename F>
        static void store(C& c, F&& f, size_t index) {
            c._results[index] = std::move(*std::forward<F>(f).get_try());
        }
    };

    template <bool Indxed, typename R>
    struct result_creator;

    template <>
    struct result_creator<true, void>
    {
        template <typename C>
        static auto go(C& context) { return context._f(context._index); }
    };

    template <>
    struct result_creator<false, void>
    {
        template <typename C>
        static auto go(C& context) { return context._f(); }
    };
    
    template <typename R>
    struct result_creator<true, R>
    {
        template <typename C>
        static auto go(C& context) { return context._f(context._results, context._index); }
    };

    template <typename R>
    struct result_creator<false, R>
    {
        template <typename C>
        static auto go(C& context) { return context._f(context._results); }
    };



    template<typename F, bool Indexed, typename R>
    struct context_result
    {
        using result_type = R;

        R                                     _results;
        boost::optional<std::exception_ptr>   _error;
        size_t                                _index;
        F                                     _f;

        context_result(F f, size_t s)
            : _f(std::move(f))
        {
            init(_results, s);
        }

        template <typename T>
        void init(std::vector<T>& v, size_t s) {
            v.resize(s);
        }

        template <typename T>
        void init(T&, size_t) {}

        template <typename FF>
        void apply(FF&& f, size_t index) {
            value_storer<R>::store(*this, std::forward<FF>(f), index);
        }

        void apply(std::exception_ptr error, size_t) {
            _error = std::move(error);
        }

        auto operator()() {
            return result_creator<Indexed,R>::go(*this);
        }
    };

    template<typename F, bool Indexed>
    struct context_result<F, Indexed, void>
    {
        boost::optional<std::exception_ptr>   _error;
        size_t                                _index;
        F                                     _f;

        context_result(F f, size_t)
            : _f(std::move(f))
        {}

        template <typename FF>
        void apply(FF&&, size_t index) {
            _index = index;
        }

        void apply(std::exception_ptr error, size_t) {
            _error = std::move(error);
        }

        auto operator()() {
            return result_creator<Indexed, void>::go(*this);
        }
    };

    /**************************************************************************************************/

    /*
     * This specialization is used for cases when only one ready future is enough to move forward.
     * In case of when_any, the first successfull future triggers the continuation. All others are cancelled.
     * In case of when_all, after the first error, this future cannot be fullfilled anymore and so we cancel the
     * all the others.
     */
    struct single_trigger
    {
        template <typename C, typename F>
        static void go(C& context, F&& f, size_t index) {
            auto before = context._single_event_trigger.test_and_set();
            if (!before) {
                for (auto& h : context._holds) h.reset();
                context.apply(std::forward<F>(f), index);
                context._f();
            }
        }
    };

    /*
    * This specialization is used for cases when all futures must be fulfilled before the continuation is triggered.
    * In case of when_any it means, that the error case handling is started, because all futures failed.
    * In case of when_all it means, that after all futures were fulfilled, the continuation is started.
    */
    struct all_trigger
    {
        template <typename C, typename F>
        static void go(C& context, F&& f, size_t index) {
            context.apply(std::forward<F>(f), index);
            if (--context._remaining == 0) context._f();
        }

        template <typename C>
        static void go(C& context, std::exception_ptr error, size_t index) {
            if (--context._remaining == 0) {
                context.apply(std::move(error), index);
                context._f();
            }
        }
    };

    template <typename CR, typename F, typename ResultCollector, typename FailureCollector>
    struct common_context : CR
    {
        std::atomic_size_t                    _remaining;
        std::atomic_flag                      _single_event_trigger = ATOMIC_FLAG_INIT;
        std::vector<future<void>>             _holds;
        packaged_task<>                       _f;

        common_context(F f, size_t s)
            : CR(std::move(f), s)
            , _remaining(s)
            , _holds(_remaining)
        {}

        auto execute() {
            if (this->_error) {
                std::rethrow_exception(this->_error.get());
            }
            return CR::operator()();
        }

        void failure(std::exception_ptr& error, size_t index) {
            FailureCollector::go(*this, error, index);
        }

        template <typename FF>
        void done(FF&& f, size_t index) {
            ResultCollector::go(*this, std::forward<FF>(f), index);
        }
    };

    /**************************************************************************************************/

    template <typename C, typename T>
    void attach_tasks(size_t index, const std::shared_ptr<C>& context, T a) {
        context->_holds[index] = std::move(a).recover([_context = std::weak_ptr<C>(context), _i = index](auto x){
            auto p = _context.lock(); if (!p) return;
            auto error = x.error();
            if (error) {
                p->failure(*error, _i);
            }
            else {
                p->done(std::move(x), _i);
            }
        });
    }

    template <typename R, typename C>
    struct create_range_of_futures {

        template<typename S, typename F, typename I>
        static auto do_it(S&& s, F&& f, I first, I last) {
            assert(first != last);

            auto context = std::make_shared<C>(std::forward<F>(f), std::distance(first, last));
            auto p = package<R()>(std::move(s), [_c = context]{
                return _c->execute();
            });

            context->_f = std::move(p.first);

            size_t index(0);
            std::for_each(first, last, [&index, &context](auto item) {
                attach_tasks(index++, context, item);
            });

            return std::move(p.second);
        }
    };

}

/**************************************************************************************************/

template <typename S, // models task scheduler
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures of the same type
auto when_all(S schedule, F f, const std::pair<I, I>& range) {
    using param_t = typename std::iterator_traits<I>::value_type::result_type;
    using result_t = typename detail::result_of_when_all_t<F, param_t>::result_type;
    using context_result_t = std::conditional_t<std::is_same<void, param_t>::value, void, std::vector<param_t>>;
    using context_t = detail::common_context<detail::context_result<F, false, context_result_t>, 
                                             F, 
                                             detail::all_trigger, 
                                             detail::single_trigger>;


    if (range.first == range.second) {
        auto p = package<result_t()>(std::move(schedule), 
                                     detail::context_result<F, false, context_result_t>(std::move(f), 0));
        schedule(std::move(p.first));
        return std::move(p.second);
    }

    return detail::create_range_of_futures<result_t,context_t>::do_it(std::move(schedule),
                                                                      std::move(f), 
                                                                      range.first, range.second);
}

/**************************************************************************************************/

template <typename S, // models task scheduler
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures of the same type
auto when_any(S schedule, F f, const std::pair<I, I>& range) {
    using param_t = typename std::iterator_traits<I>::value_type::result_type;
    using result_t = typename detail::result_of_when_any_t<F, param_t>::result_type;
    using context_result_t = std::conditional_t<std::is_same<void, param_t>::value, void, param_t>;
    using context_t = detail::common_context<detail::context_result<F, true, context_result_t>, 
                                             F, 
                                             detail::single_trigger, 
                                             detail::all_trigger>;

    if (range.first == range.second) {
        auto p = package_with_broken_promise<result_t()>(std::move(schedule), 
                                                         detail::context_result<F, true, context_result_t>(std::move(f), 0));
        return std::move(p.second);
    }

    return detail::create_range_of_futures<result_t, context_t>::do_it(std::move(schedule),
                                                                       std::move(f),
                                                                       range.first, range.second);
}

/**************************************************************************************************/

template <typename S, typename F, typename ...Args>
auto async(S schedule, F&& f, Args&&... args)
        -> future<std::result_of_t<F (Args...)>>
{
    auto p = package<std::result_of_t<F(Args...)>()>(schedule,
        std::bind([_f = std::forward<F>(f)](Args&... args) {
            return _f(std::move(args)...);
        }, std::forward<Args>(args)...));
    
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
    for (const auto& e : then) 
        e.first(e.second);
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
void async_(std::chrono::system_clock::time_point, std::function<void()>);

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

struct default_scheduler {
    using result_type = void;


    template <typename F>
    void operator()(std::chrono::system_clock::time_point when, F f) {

        using namespace std::chrono;

        if (when == system_clock::time_point()) {
            operator()(std::move(f));
            return;
        }

        using f_t = decltype(f);

        dispatch_after_f(dispatch_time(0, duration_cast<nanoseconds>(when - system_clock::now()).count()),
                dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                new f_t(std::move(f)),
                [](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                });
    }

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        dispatch_async_f(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                new f_t(std::move(f)),
                [](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                });
    }
};

struct main_scheduler {
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        dispatch_async_f(dispatch_get_main_queue(),
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

using main_scheduler = default_scheduler;

#elif  (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS)


struct default_scheduler {
    using result_type = void;

    template <typename F>
    void operator()(std::chrono::system_clock::time_point when, F f) {
        detail::async_(when, std::move(f));
    }

    template <typename F>
    void operator()(F f) {
        detail::async_(std::move(f));
    }
};

// TODO (sparent) : We need a main task scheduler for STLAB_TASK_SYSTEM_WINDOWS

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL

struct main_scheduler {
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        pp::Module::Get()->core()->CallOnMainThread(0,
            pp::CompletionCallback([](void* f_, int32_t) {
                auto f = static_cast<f_t*>(f_);
                (*f)();
                delete f;
            }, new f_t(std::move(f))), 0);
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

class task_system
{
public:
    template <typename F>
    void async_(F&& f) {
        auto work = CreateThreadpoolWork(&callback_impl<F>,
            new F(std::forward<F>(f)),
            nullptr);
        if (work == nullptr) {
            throw std::bad_alloc();
        }
        SubmitThreadpoolWork(work);
    }

    template <typename F>
    void async_(std::chrono::system_clock::time_point when, F&& f) {

        auto timer = CreateThreadpoolTimer(&timer_callback_impl<F>,
            new F(std::forward<F>(f)),
            nullptr);
        if (timer == nullptr) {
            throw std::bad_alloc();
        }

        auto file_time = time_point_to_FILETIME(when);

        SetThreadpoolTimer(timer,
            &file_time,
            0,
            0);
    }
private:

    template <typename F>
    static void CALLBACK callback_impl(PTP_CALLBACK_INSTANCE /*instance*/,
        PVOID                 parameter,
        PTP_WORK              /*Work*/) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
    }

    template <typename F>
    static void CALLBACK timer_callback_impl(PTP_CALLBACK_INSTANCE /*Instance*/,
        PVOID                 parameter,
        PTP_TIMER             /*timer*/) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
    }

    FILETIME time_point_to_FILETIME(const std::chrono::system_clock::time_point& when) {
        FILETIME ft = { 0, 0 };
        SYSTEMTIME st = { 0 };
        time_t t = std::chrono::system_clock::to_time_t(when);
        tm utc_tm;
        if (!gmtime_s(&utc_tm, &t)) {
            st.wSecond = static_cast<WORD>(utc_tm.tm_sec);
            st.wMinute = static_cast<WORD>(utc_tm.tm_min);
            st.wHour = static_cast<WORD>(utc_tm.tm_hour);
            st.wDay = static_cast<WORD>(utc_tm.tm_mday);
            st.wMonth = static_cast<WORD>(utc_tm.tm_mon + 1);
            st.wYear = static_cast<WORD>(utc_tm.tm_year + 1900);
            st.wMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(when.time_since_epoch()).count() % 1000;
            SystemTimeToFileTime(&st, &ft);
        }
        return ft;
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

// TODO (sparent) : provide a scheduler and run-loop - this is provide for testing on mac
struct main_scheduler {
    using result_type = void;

    #if __APPLE__
    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        ::dispatch_async_f(dispatch_get_main_queue(),
                new f_t(std::move(f)), [](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                });
    }
    #endif
};
#endif

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
