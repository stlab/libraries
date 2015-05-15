/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/optional.hpp>
#include <future>
#include <initializer_list>
#include <memory>
#include <thread>
#include <vector>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> { using type = R; };

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename> class packaged_task;
template <typename> class future;

using schedule_t = std::function<void(std::function<void()>)>;

template <typename Signature, typename S, typename F>
auto package(S, F)
    -> std::pair<packaged_task<Signature>, future<detail::result_of_t_<Signature>>>;

/**************************************************************************************************/

namespace detail {

template <typename> struct shared;
template <typename, typename = void> struct shared_base;

template <typename T>
using enable_if_copyable = std::enable_if_t<std::is_copy_assignable<T>::value>;

template <typename T>
using enable_if_not_copyable = std::enable_if_t<!std::is_copy_assignable<T>::value>;

template <typename T>
struct shared_base<T, enable_if_copyable<T>> : std::enable_shared_from_this<shared_base<T>> {
    schedule_t                          _schedule;
    boost::optional<T>                  _result;
    boost::optional<std::exception_ptr> _error;
    std::mutex                          _mutex;
    bool                                _ready = false;
    std::vector<std::function<void()>>  _then;

    explicit shared_base(schedule_t s) : _schedule(std::move(s)) { }

    template <typename F>
    auto then(F f) { return then(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto then(S s, F f) {
        auto p = package<typename std::result_of<F(T)>::type()>(std::move(s),
            [_f = std::move(f), _p = this->shared_from_this()] {
                if (_p->_error) std::rethrow_exception(_p->_error.get());
                return _f(_p->_result.get());
            });
        std::unique_lock<std::mutex> lock(_mutex);
        if (_ready) _schedule(std::move(p.first));
        else _then.emplace_back(std::move(p.first));
        return std::move(p.second);
    }

    template <typename F>
    auto then_r(bool unique, F f) { return then_r(unique, _schedule, std::move(f)); }

    template <typename S, typename F>
    auto then_r(bool unique, S s, F f) {
        if (!unique) return then(std::move(s), std::move(f));

        auto p = package<typename std::result_of<F(T)>::type()>(std::move(s),
            [_f = std::move(f), _p = this->shared_from_this()] {
                if (_p->_error) std::rethrow_exception(_p->_error.get());
                return _f(std::move(_p->_result.get()));
            });

        {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_ready) _schedule(std::move(p.first));
        else _then.emplace_back(std::move(p.first));
        }

        return std::move(p.second);
    }

    template <typename F>
    auto error(F f) { // COMPLETE
    }

    void set_exception(std::exception_ptr error) {
        _error = std::move(error);
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
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

template <typename T>
struct shared_base<T, enable_if_not_copyable<T>> : std::enable_shared_from_this<shared_base<T>> {
    schedule_t                          _schedule;
    boost::optional<T>                  _result;
    boost::optional<std::exception_ptr> _error;
    std::mutex                          _mutex;
    bool                                _ready = false;
    std::function<void()>               _then;

    explicit shared_base(schedule_t s) : _schedule(std::move(s)) { }

    template <typename F>
    auto then(F f) { return then_r(true, _schedule, std::move(f)); }

    template <typename S, typename F>
    auto then(S s, F f) {
        return then_r(true, std::move(s), std::move(f));
    }

    template <typename F>
    auto then_r(bool unique, F f) { return then_r(unique, _schedule, std::move(f)); }

    template <typename S, typename F>
    auto then_r(bool, S s, F f) {
        auto p = package<typename std::result_of<F(T)>::type()>(std::move(s),
            [_f = std::move(f), _p = this->shared_from_this()] {
                if (_p->_error) std::rethrow_exception(_p->_error.get());
                return _f(std::move(_p->_result.get()));
            });

        {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_ready) _schedule(std::move(p.first));
        else _then =std::move(p.first);
        }

        return std::move(p.second);
    }

    template <typename F>
    auto error(F f) { // COMPLETE
    }

    void set_exception(std::exception_ptr error) {
        _error = std::move(error);
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
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

template <>
struct shared_base<void> : std::enable_shared_from_this<shared_base<void>> {
    schedule_t                          _schedule;
    boost::optional<std::exception_ptr> _error;
    std::mutex                          _mutex;
    bool                                _ready = false;
    std::vector<std::function<void()>>  _then;

    explicit shared_base(schedule_t s) : _schedule(std::move(s)) { }

    template <typename F>
    auto then(F f) { return then(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto then(S s, F f) {
        auto p = package<typename std::result_of<F()>::type()>(std::move(s),
            [_f = std::move(f), _p = this->shared_from_this()] {
                if (_p->_error) std::rethrow_exception(_p->_error.get());
                _f();
            });
        std::unique_lock<std::mutex> lock(_mutex);
        if (_ready) _schedule(std::move(p.first));
        else _then.emplace_back(std::move(p.first));
        return std::move(p.second);
    }

    template <typename F>
    auto then_r(bool, F f) { return then(_schedule, std::move(f)); }

    template <typename S, typename F>
    auto then_r(bool, S s, F f) { return then(std::move(s), std::move(f)); }

    template <typename F>
    auto error(F f) { // COMPLETE
    }

    void set_exception(std::exception_ptr error) {
        _error = std::move(error);
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
    }

    auto get_try() {
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
struct shared<R (Args...)> : shared_base<R>
{
    using function_t = std::function<R (Args...)>;

    std::atomic_size_t _promise_count;
    function_t _f;

    template <typename F>
    explicit shared(schedule_t s, F f) : shared_base<R>(std::move(s)), _f(std::move(f)) {
        _promise_count = 1;
    }

    void remove_promise() {
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
    void add_promise() { ++_promise_count; }

    template <typename... A>
    void operator()(A&&... args) {
        if (_f) try {
            this->set_value(_f, std::forward<A>(args)...);
        } catch(...) {
            this->set_exception(std::current_exception());
        }
        _f = function_t();
    }
};

/**************************************************************************************************/

} // detail

/**************************************************************************************************/

template<typename R, typename ...Args >
class packaged_task<R (Args...)> {
    using ptr_t = std::weak_ptr<detail::shared<R (Args...)>>;

    ptr_t _p;

    explicit packaged_task(ptr_t p) : _p(std::move(p)) { }

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<packaged_task<Signature>, future<detail::result_of_t_<Signature>>>;
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

template <typename T>
class future {
    using ptr_t = std::shared_ptr<detail::shared_base<T>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) { }

    template <typename Signature, typename S, typename F>
    friend auto package(S, F)
        -> std::pair<packaged_task<Signature>, future<detail::result_of_t_<Signature>>>;

  public:
    future() = default;

    template <typename F>
    auto then(F&& f) const& { return _p->then(std::forward<F>(f)); }

    template <typename S, typename F>
    auto then(S&& s, F&& f) const& { return _p->then(std::forward<S>(s), std::forward<F>(f)); }

    template <typename F>
    auto then(F&& f) && { return _p->then_r(_p.unique(), std::forward<F>(f)); }

    template <typename S, typename F>
    auto then(S&& s, F&& f) && {
        return _p->then_r(_p.unique(), std::forward<S>(s), std::forward<F>(f));
    }

    bool cancel_try() {
        if (!_p.unique()) return false;
        std::weak_ptr<detail::shared_base<T>> p = _p;
        _p.reset();
        _p = p.lock();
        return !_p;
    }

    auto get_try() const& { return _p->get_try(); }
    auto get_try() && { return _p->get_try_r(_p.unique()); }
};

template <typename Sig, typename S, typename F>
auto package(S s, F f) -> std::pair<packaged_task<Sig>, future<detail::result_of_t_<Sig>>> {
    auto p = std::make_shared<detail::shared<Sig>>(std::move(s), std::move(f));
    return std::make_pair(packaged_task<Sig>(p), future<detail::result_of_t_<Sig>>(p));
}

/**************************************************************************************************/

namespace detail {

template <typename F, typename... Ts>
struct when_all_shared {
    // decay
    std::tuple<boost::optional<Ts>...>      _args;
    future<void>                            _holds[sizeof...(Ts)] {};
    std::atomic_size_t                      _remaining {sizeof...(Ts)};
    packaged_task<std::result_of_t<F(Ts...)>()> _f;

    void done() { if (--_remaining == 0) _f(); }
};

template <typename F, typename Args, std::size_t... I>
auto apply_when_all_args_(const F& f, Args& args, std::index_sequence<I...>) {
    return f(std::move(std::get<I>(args).get())...);
}

template <typename F, typename Args>
auto apply_when_all_args(const F& f, Args& args) {
    return apply_when_all_args_(f, args, std::make_index_sequence<std::tuple_size<Args>::value>());
}

template <typename P, typename... Ts, std::size_t... I>
void attach_when_all_args_(std::index_sequence<I...>, const std::shared_ptr<P>& p, Ts... a) {
    std::weak_ptr<P> w = p;
    (void)std::initializer_list<int>{(p->_holds[I] = std::move(a).then([_w = w](auto x){
        auto p = _w.lock(); if (!p) return;
        std::get<I>(p->_args) = std::move(x);
        p->done();
    }), 0)...};
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
        return detail::apply_when_all_args(_f, _p->_args);
    });
    shared->_f = std::move(p.first);

    detail::attach_when_all_args(shared, std::move(args)...);

    return std::move(p.second);
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
    std::vector<std::function<void()>> then;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
        then = std::move(_then);
    }
    for (const auto& e : then) _schedule(e);
}

template <typename T>
template <typename F, typename... Args>
void shared_base<T, enable_if_copyable<T>>::set_value(const F& f, Args&&... args) {
    _result = f(std::forward<Args>(args)...);
    std::vector<std::function<void()>> then;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
        then = std::move(_then);
    }
    for (const auto& e : then) _schedule(e);
}

template <typename T>
template <typename F, typename... Args>
void shared_base<T, enable_if_not_copyable<T>>::set_value(const F& f, Args&&... args) {
    _result = f(std::forward<Args>(args)...);
    std::function<void()> then;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _ready = true;
        then = std::move(_then);
    }
    if (then) _schedule(then);
}

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/
