/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_FUTURE_HPP
#define STLAB_CONCURRENCY_FUTURE_HPP

#include <stlab/config.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <exception>
#include <functional>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <optional>
#include <variant> // for std::monostate
#include <vector>

#ifndef STLAB_NO_STD_COROUTINES
#include <coroutine>
#endif

#include <stlab/concurrency/executor_base.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/task.hpp>
#include <stlab/concurrency/traits.hpp>
#include <stlab/concurrency/tuple_algorithm.hpp>
#include <stlab/functional.hpp>
#include <stlab/memory.hpp>
#include <stlab/utility.hpp>

/**************************************************************************************************/

/*
    packaged_task<Args...> is a function that holds a type erased promise
    invoking the packaged task will call with the moved promise

*/

/**************************************************************************************************/

namespace stlab {
inline namespace STLAB_VERSION_NAMESPACE() {

/**************************************************************************************************/

// invoke mapping void to std::monostate
template <class F, class... Args>
auto invoke_void_to_monostate_result(F&& f, Args&&... args) {
    if constexpr (std::is_void_v<std::invoke_result_t<F, Args...>>) {
        std::forward<F>(f)(std::forward<Args>(args)...);
        return std::monostate{};
    } else {
        return std::forward<F>(f)(std::forward<Args>(args)...);
    }
}

// REVISIT (sean-parent) : As a typedef, this generates file names in the hyde documentation that
// are too long for windows. Moving to a class for now, but I may also change how this is used and
// have a single future<> class with conditional members for easier documentation.
template <class T>
struct void_to_monostate {
    using type = std::conditional_t<std::is_void_v<T>, std::monostate, T>;
};

template <class T>
using void_to_monostate_t = typename void_to_monostate<T>::type;

template <class T>
inline constexpr bool is_monostate_v = std::is_same_v<T, std::monostate>;

template <class T>
auto optional_monostate_to_bool(std::optional<T>&& o) {
    if constexpr (is_monostate_v<T>) {
        return o.has_value();
    } else {
        return std::move(o);
    }
}

template <class T>
auto monostate_to_void(T&& a) {
    if constexpr (is_monostate_v<T>) {
        return;
    } else {
        return std::forward<T>(a);
    }
}

template <class T>
auto monostate_to_empty_tuple(T&& a) {
    if constexpr (is_monostate_v<T>) {
        return std::tuple{};
    } else {
        return std::forward_as_tuple(std::forward<T>(a));
    }
}

template <class F, class... Args>
auto invoke_remove_monostate_arguments(F&& f, Args&&... args) {
    return std::apply(
        [&](auto&&... args) { return std::forward<F>(f)(std::forward<decltype(args)>(args)...); },
        std::apply(
            [&](auto&&... args) {
                return std::tuple_cat(monostate_to_empty_tuple(std::forward<Args>(args))...);
            },
            std::forward_as_tuple(std::forward<Args>(args)...)));
}

/**************************************************************************************************/

enum class future_error_codes : std::uint8_t { // names for futures errors
    broken_promise = 1,
    no_state
};

/**************************************************************************************************/

namespace detail {

inline auto Future_error_map(future_error_codes code) noexcept -> const
    char* {         // convert to name of future error
    switch (code) { // switch on error code value
        case future_error_codes::broken_promise:
            return "broken promise";

        case future_error_codes::no_state:
            return "no state";

        default:
            return nullptr;
    }
}

/**************************************************************************************************/

// This could be lifted into a common header if needed in other places
#if __cplusplus < 201703L
template <class F, class... Args>
using result_t = std::result_of_t<F(Args...)>;
#else
template <class F, class... Args>
using result_t = std::invoke_result_t<F, Args...>;
#endif

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

// future exception

class future_error : public std::logic_error {
public:
    explicit future_error(future_error_codes code) : logic_error(""), _code(code) {}

    [[nodiscard]] auto code() const noexcept -> const future_error_codes& { return _code; }

    [[nodiscard]] auto what() const noexcept -> const char* override {
        return detail::Future_error_map(_code);
    }

private:
    const future_error_codes _code; // the stored error code
};

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> {
    using type = R;
};

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

template <typename F, typename T>
struct result_of_when_all_t;

template <typename F>
struct result_of_when_all_t<F, void> {
    using result_type = detail::result_t<F>;
};

template <typename F, typename T>
struct result_of_when_all_t {
    using result_type = detail::result_t<F, const std::vector<T>&>;
};

template <typename F, typename T>
struct result_of_when_any_t;

template <typename F>
struct result_of_when_any_t<F, void> {
    using result_type = detail::result_t<F, size_t>;
};

template <typename F, typename R>
struct result_of_when_any_t {
    using result_type = detail::result_t<F, R, size_t>;
};

template <typename T>
auto unique_usage(const std::shared_ptr<T>& p) -> bool {
    return p.use_count() == 1;
}

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename...>
class packaged_task;

template <typename, typename = void>
class future;

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <typename>
struct packaged_task_from_signature;

template <typename R, typename... Args>
struct packaged_task_from_signature<R(Args...)> {
    using type = packaged_task<Args...>;
};

template <typename T>
using packaged_task_from_signature_t = typename packaged_task_from_signature<T>::type;

/**************************************************************************************************/

template <class>
struct reduced_signature;

template <class R, class... Args>
struct reduced_signature<R(Args...)> {
    using type = R(Args...);
};

template <class R, class... Args>
struct reduced_signature<future<R>(Args...)> {
    using type = R(Args...);
};

template <class T>
using reduced_signature_t = typename reduced_signature<T>::type;

/**************************************************************************************************/

template <class T>
inline constexpr bool is_future_v = false;

template <class T>
inline constexpr bool is_future_v<future<T>> = true;

template <typename T>
using reduced_t = std::conditional_t<is_future_v<T>, T, future<T>>;

template <typename Sig>
using reduced_result_t = reduced_t<result_of_t_<Sig>>;

/**************************************************************************************************/

template <typename T, typename = void>
struct value_;

} // namespace detail

/**************************************************************************************************/

template <typename Sig, typename E, typename F>
auto package(E, F&&)
    -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>>;

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <class>
struct shared;
template <class, class = void>
struct shared_base;

/**************************************************************************************************/

template <typename... Args>
struct shared_task {
    virtual ~shared_task() = default;

    virtual void operator()(Args... args) = 0;
    virtual void set_exception(const std::exception_ptr& error) = 0;
};

/**************************************************************************************************/

template <typename T>
struct shared_base<T, enable_if_copyable<void_to_monostate_t<T>>>
    : std::enable_shared_from_this<shared_base<T>> {
    using then_t = std::vector<std::pair<executor_t, task<void() noexcept>>>;

    using type = void_to_monostate_t<T>;

    executor_t _executor;
    std::optional<type> _result;
    std::exception_ptr _exception;
    std::mutex _mutex;
    std::atomic_bool _ready{false};
    then_t _then;

    explicit shared_base(executor_t s) : _executor(std::move(s)) {}

    template <typename F>
    auto recover(future<T>&& p, F&& f) {
        return recover(std::move(p), _executor, std::forward<F>(f));
    }

    template <typename E, typename F>
    auto recover(future<T>&& p, E executor, F&& f) {
        using result_type = detail::result_t<F, future<T>>;

        auto [pro, fut] = package<result_type()>(
            executor, [_f = std::forward<F>(f), _p = std::move(p)]() mutable {
                return std::move(_f)(std::move(_p));
            });

        bool ready{false};
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) _then.emplace_back(std::move(executor), std::move(pro));
        }

        if (ready) executor(std::move(pro)); // cannot reference this after here

        return std::move(fut);
    }

    template <class F>
    void _detach(future<T>&& p, F&& f) {
        auto pro = [_f = std::forward<F>(f), _p = std::move(p)]() mutable noexcept {
            std::move(_f)(std::move(_p));
        };

        bool ready{false};
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) _then.emplace_back(immediate_executor, std::move(pro));
        }

        if (ready) std::move(pro)(); // cannot reference this after here
    }

    void _detach() {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_ready)
            _then.emplace_back([](auto&&) {}, [_p = this->shared_from_this()]() noexcept {});
    }

    void _set_exception(const std::exception_ptr& error) noexcept {
        _exception = error;
        then_t then;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            then = std::move(_then);
            _ready = true;
        }
        // propagate exception with scheduling
        for (auto& e : then) {
            e.first(std::move(e.second));
        }
    }

    template <class A>
    void set_value(A&& a);

    auto is_ready() const& -> bool { return _ready; }

    // get_ready() is called internally on continuations when we know _ready is true;
    auto get_ready() -> const type& {
        assert(is_ready() && "FATAL (sean.parent) : get_ready() called but not ready!");

        if (_exception) std::rethrow_exception(_exception);
        return *_result;
    }

    auto get_ready_r(bool unique) -> type {
        if (!unique) return get_ready();

        assert(is_ready() && "FATAL (sean.parent) : get_ready() called but not ready!");

        if (_exception) std::rethrow_exception(_exception);
        return std::move(*_result);
    }

    auto get_try() -> std::optional<type> {
        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_exception) std::rethrow_exception(_exception);
            return _result;
        }
        return std::nullopt;
    }

    auto get_try_r(bool unique) -> std::optional<type> {
        if (!unique) return get_try();

        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_exception) std::rethrow_exception(_exception);
            return std::move(_result);
        }
        return std::nullopt;
    }
};

/**************************************************************************************************/

template <typename T>
struct shared_base<T, enable_if_not_copyable<void_to_monostate_t<T>>>
    : std::enable_shared_from_this<shared_base<T>> {
    using then_t = std::pair<executor_t, task<void() noexcept>>;

    executor_t _executor;
    std::optional<T> _result;
    std::exception_ptr _exception;
    std::mutex _mutex;
    std::atomic_bool _ready{false};
    then_t _then;

    explicit shared_base(executor_t s) : _executor(std::move(s)) {}

    template <typename F>
    auto recover(future<T>&& p, F&& f) {
        return recover(std::move(p), _executor, std::forward<F>(f));
    }

    /*
        NOTE : executor cannot be a reference type here. When invoked it could
        cause _this_ to be deleted, and the executor passed in may be
        this->_executor
    */
    template <typename E, typename F>
    auto recover(future<T>&& p, E executor, F&& f) {
        using result_type = detail::result_t<F, future<T>>;

        auto [pro, fut] = package<result_type()>(
            executor, [_f = std::forward<F>(f), _p = std::move(p)]() mutable {
                return std::move(_f)(std::move(_p));
            });

        bool ready{false};
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) _then = {std::move(executor), std::move(pro)};
        }
        if (ready) executor(std::move(pro)); // cannot reference this after here

        return std::move(fut);
    }

    template <class F>
    void _detach(future<T>&& p, F&& f) {
        auto pro = [_f = std::forward<F>(f), _p = std::move(p)]() mutable noexcept {
            std::move(_f)(std::move(_p));
        };

        bool ready{false};
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
            if (!ready) _then = {immediate_executor, std::move(pro)};
        }

        if (ready) std::move(pro)(); // cannot reference this after here
    }

    void _detach() {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_ready) _then = then_t([](auto&&) {}, [_p = this->shared_from_this()]() noexcept {});
    }

    void _set_exception(const std::exception_ptr& error) noexcept {
        _exception = error;
        then_t then;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_then.second) then = std::move(_then);
            _ready = true;
        }
        // propagate exception with scheduling
        if (then.second) then.first(std::move(then.second));
    }

    template <class A>
    void set_value(A&& a);

    auto is_ready() const -> bool {
        return _ready;
    } // get_ready() is called internally on continuations when we know _ready is true;

    auto get_ready() -> const T& { return get_ready_r(true); }

    auto get_ready_r(bool) -> T {
        assert(is_ready() && "FATAL (sean.parent) : get_ready() called but not ready!");

        if (_exception) std::rethrow_exception(_exception);
        return std::move(*_result);
    }

    auto get_try() -> std::optional<T> { return get_try_r(true); }

    auto get_try_r(bool) -> std::optional<T> {
        bool ready = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            ready = _ready;
        }
        if (ready) {
            if (_exception) std::rethrow_exception(_exception);
            return std::move(_result);
        }
        return {};
    }
};

/**************************************************************************************************/

// class promise

/**************************************************************************************************/

template <class R>
class promise {
    using type = void_to_monostate_t<R>;
    std::weak_ptr<shared_base<R>> _p;

public:
    explicit promise(const std::shared_ptr<shared_base<R>>& p) : _p{p} {}

    ~promise() {
        if (auto p = _p.lock()) {
            p->_set_exception(
                std::make_exception_ptr(future_error(future_error_codes::broken_promise)));
        }
    }

    promise(promise&&) noexcept = default;
    auto operator=(promise&&) noexcept -> promise& = default;

    promise(const promise&) = delete;
    auto operator=(const promise&) -> promise& = delete;

    void set_value(type&& value) && noexcept {
        if (auto p = _p.lock()) {
            _p.reset();
            p->set_value(std::move(value));
        }
    }

    auto set_value() && noexcept { set_value(std::monostate{}); }

    void set_exception(const std::exception_ptr& error) && noexcept {
        if (auto p = _p.lock()) {
            _p.reset();
            p->_set_exception(error);
        }
    }

    [[nodiscard]] auto canceled() const -> bool { return _p.expired(); }
};

template <class R>
promise(std::shared_ptr<shared_base<R>>) -> promise<R>;

template <typename R, typename... Args>
struct shared<R(Args...)> final : shared_base<R>, shared_task<Args...> {
    using function_t = task<void(promise<R>&&, Args...) noexcept>;

    function_t _f;

    template <typename F>
    shared(executor_t s, F&& f) : shared_base<R>(std::move(s)), _f(std::forward<F>(f)) {}

    shared(executor_t s) : shared_base<R>(std::move(s)) {}

    void operator()(Args... args) noexcept override {
        if (!_f) return;

        _f(promise{this->shared_from_this()}, std::move(args)...);

        _f = function_t();
    }

    void set_exception(const std::exception_ptr& error) override {
        if (!_f) return;

        this->_set_exception(error);
        _f = function_t();
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename... Args>
class packaged_task {
    using ptr_t = std::weak_ptr<detail::shared_task<Args...>>;
    ptr_t _p;

    explicit packaged_task(ptr_t&& p) : _p(std::move(p)) {}

    template <typename Sig, typename E, typename F>
    friend auto package(E, F&&)
        -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>>;

    template <class T, class E>
    friend auto future_with_broken_promise(E) -> detail::reduced_t<T>;

public:
    packaged_task() = default;
    ~packaged_task() {
        if (auto p = _p.lock()) {
            p->set_exception(
                std::make_exception_ptr(future_error(future_error_codes::broken_promise)));
        }
    }

    packaged_task(const packaged_task&) = delete;
    auto operator=(const packaged_task&) -> packaged_task& = delete;

    packaged_task(packaged_task&&) noexcept = default;
    auto operator=(packaged_task&& x) noexcept -> packaged_task& = default;

    template <typename... A>
    void operator()(A&&... args) noexcept {
        if (auto p = _p.lock()) {
            _p.reset();
            (*p)(std::forward<A>(args)...);
        }
    }

    [[nodiscard]] auto canceled() const -> bool { return _p.expired(); }

    void set_exception(const std::exception_ptr& error) noexcept {
        if (auto p = _p.lock()) {
            _p.reset();
            p->set_exception(error);
        }
    }
};

/**************************************************************************************************/

template <typename T>
class STLAB_NODISCARD() future<T, enable_if_copyable<void_to_monostate_t<T>>> {
    using type = void_to_monostate_t<T>;
    using ptr_t = std::shared_ptr<detail::shared_base<T>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) {}

    template <typename Sig, typename E, typename F>
    friend auto package(E, F&&)
        -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>>;

    template <class U, class E>
    friend auto future_with_broken_promise(E) -> detail::reduced_t<U>;

    friend struct detail::shared_base<type>;

    template <typename, typename>
    friend struct detail::value_;

public:
    using result_type = T;

    future() = default;

    void swap(future& x) noexcept { std::swap(_p, x._p); }

    inline friend void swap(future& x, future& y) noexcept { x.swap(y); }
    inline friend auto operator==(const future& x, const future& y) -> bool { return x._p == y._p; }
    inline friend auto operator!=(const future& x, const future& y) -> bool { return !(x == y); }

    [[nodiscard]] auto valid() const -> bool { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) const& {
        return recover([_f = std::forward<F>(f)](future<result_type>&& p) mutable {
            return invoke_remove_monostate_arguments(
                std::move(_f),
                invoke_void_to_monostate_result([&] { return std::move(p).get_ready(); }));
        });
    }

    template <typename F>
    auto operator|(F&& f) const& {
        return then(std::forward<F>(f));
    }

    template <typename E, typename F>
    auto then(E&& executor, F&& f) const& {
        return recover(
            std::forward<E>(executor), [_f = std::forward<F>(f)](future<result_type>&& p) mutable {
                return invoke_remove_monostate_arguments(
                    std::move(_f),
                    invoke_void_to_monostate_result([&] { return std::move(p).get_ready(); }));
            });
    }

    template <typename F>
    auto operator|(executor_task_pair<F> etp) const& {
        return then(std::move(etp)._executor, std::move(etp)._f);
    }

    template <typename F>
    auto then(F&& f) && {
        return std::move(*this).recover([_f = std::forward<F>(f)](future<result_type>&& p) mutable {
            return invoke_remove_monostate_arguments(
                std::move(_f),
                invoke_void_to_monostate_result([&] { return std::move(p).get_ready(); }));
        });
    }

    template <typename F>
    auto operator|(F&& f) && {
        return std::move(*this).then(std::forward<F>(f));
    }

    template <typename E, typename F>
    auto then(E&& executor, F&& f) && {
        return std::move(*this).recover(
            std::forward<E>(executor), [_f = std::forward<F>(f)](future<result_type>&& p) mutable {
                return invoke_remove_monostate_arguments(
                    std::move(_f),
                    invoke_void_to_monostate_result([&] { return std::move(p).get_ready(); }));
            });
    }

    template <typename F>
    auto operator|(executor_task_pair<F> etp) && {
        return std::move(*this).then(std::move(etp)._executor, std::move(etp)._f);
    }

    template <typename F>
    auto recover(F&& f) const& {
        return _p->recover(copy(*this), std::forward<F>(f));
    }

    template <typename F>
    auto operator^(F&& f) const& {
        return recover(std::forward<F>(f));
    }

    template <typename E, typename F>
    auto recover(E&& executor, F&& f) const& {
        return _p->recover(copy(*this), std::forward<E>(executor), std::forward<F>(f));
    }

    template <typename F>
    auto operator^(executor_task_pair<F> etp) const& {
        return recover(std::move(etp)._executor, std::move(etp)._f);
    }

    template <typename F>
    auto recover(F&& f) && {
        auto& self = *_p.get();
        return self.recover(std::move(*this), std::forward<F>(f));
    }

    template <typename F>
    auto operator^(F&& f) && {
        return std::move(*this).recover(std::forward<F>(f));
    }

    template <typename E, typename F>
    auto recover(E&& executor, F&& f) && {
        auto& self = *_p.get();
        return self.recover(std::move(*this), std::forward<E>(executor), std::forward<F>(f));
    }

    template <typename F>
    auto operator^(executor_task_pair<F> etp) && {
        return std::move(*this).recover(std::move(etp)._executor, std::move(etp)._f);
    }

    void detach() const { _p->_detach(); }

    template <class F>
    void detach(F&& f) && {
        auto& self = *_p.get();
        self._detach(std::move(*this), std::forward<F>(f));
    }

    void reset() { _p.reset(); }

    [[nodiscard]] auto is_ready() const& -> bool { return _p && _p->is_ready(); }

    [[nodiscard]] auto get_try() const& { return optional_monostate_to_bool(_p->get_try()); }

    auto get_try() && { return optional_monostate_to_bool(_p->get_try_r(unique_usage(_p))); }

    [[nodiscard]] auto get_ready() const& { return monostate_to_void(_p->get_ready()); }

    auto get_ready() && { return monostate_to_void(_p->get_ready_r(unique_usage(_p))); }

    [[deprecated("Use exception() instead")]] [[nodiscard]] auto error()
        const& -> std::optional<std::exception_ptr> {
        return _p->_exception ? std::optional<std::exception_ptr>{_p->_exception} : std::nullopt;
    }

    // Precondition: is_ready()
    [[nodiscard]] auto exception() const& -> std::exception_ptr {
        assert(is_ready());
        return _p->_exception;
    }
};

/**************************************************************************************************/

template <typename T>
class STLAB_NODISCARD() future<T, enable_if_not_copyable<void_to_monostate_t<T>>> {
    using ptr_t = std::shared_ptr<detail::shared_base<T>>;
    ptr_t _p;

    explicit future(ptr_t p) : _p(std::move(p)) {}
    future(const future&) = default;

    template <typename Sig, typename E, typename F>
    friend auto package(E, F&&)
        -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>>;

    template <class U, class E>
    friend auto future_with_broken_promise(E) -> detail::reduced_t<U>;

    friend struct detail::shared_base<T>;

    template <typename, typename>
    friend struct detail::value_;

public:
    using result_type = T;

    future() = default;
    future(future&&) noexcept = default;
    auto operator=(const future&) -> future& = delete;
    auto operator=(future&&) noexcept -> future& = default;

    void swap(future& x) noexcept { std::swap(_p, x._p); }

    inline friend void swap(future& x, future& y) noexcept { x.swap(y); }
    inline friend auto operator==(const future& x, const future& y) -> bool { return x._p == y._p; }
    inline friend auto operator!=(const future& x, const future& y) -> bool { return !(x == y); }

    [[nodiscard]] auto valid() const -> bool { return static_cast<bool>(_p); }

    template <typename F>
    auto then(F&& f) && {
        return std::move(*this).recover([_f = std::forward<F>(f)](future<result_type>&& p) mutable {
            return std::move(_f)(*std::move(p).get_try());
        });
    }

    template <typename F>
    auto operator|(F&& f) && {
        return std::move(*this).then(std::forward<F>(f));
    }

    template <typename E, typename F>
    auto then(E&& executor, F&& f) && {
        return std::move(*this).recover(std::forward<E>(executor),
                                        [_f = std::forward<F>(f)](future<result_type>&& p) mutable {
                                            return std::move(_f)(*std::move(p).get_try());
                                        });
    }

    template <typename F>
    auto operator|(executor_task_pair<F> etp) && {
        return std::move(*this).then(std::move(etp)._executor, std::move(etp)._f);
    }

    template <typename F>
    auto recover(F&& f) && {
        auto& self = *_p.get();
        return self.recover(std::move(*this), std::forward<F>(f));
    }

    template <typename F>
    auto operator^(F&& f) && {
        return std::move(*this).recover(std::forward<F>(f));
    }

    template <typename E, typename F>
    auto recover(E&& executor, F&& f) && {
        auto& self = *_p.get();
        return self.recover(std::move(*this), std::forward<E>(executor), std::forward<F>(f));
    }

    template <typename F>
    auto operator^(executor_task_pair<F> etp) && {
        return std::move(*this).recover(std::move(etp)._executor, std::move(etp)._f);
    }

    void detach() const { _p->_detach(); }

    template <class F>
    void detach(F&& f) && {
        auto& self = *_p.get();
        self._detach(std::move(*this), std::forward<F>(f));
    }

    void reset() { _p.reset(); }

    [[nodiscard]] auto is_ready() const& -> bool { return _p && _p->is_ready(); }

    [[nodiscard]] auto get_try() const& { return optional_monostate_to_bool(_p->get_try()); }

    auto get_try() && { return optional_monostate_to_bool(_p->get_try_r(unique_usage(_p))); }

    [[nodiscard]] auto get_ready() const& { return monostate_to_void(_p->get_ready()); }

    auto get_ready() && { return monostate_to_void(_p->get_ready_r(unique_usage(_p))); }

    [[deprecated("Use exception() instead")]] [[nodiscard]] auto error()
        const& -> std::optional<std::exception_ptr> {
        return _p->_exception ? std::optional<std::exception_ptr>{_p->_exception} : std::nullopt;
    }

    // Precondition: is_ready()
    [[nodiscard]] auto exception() const& -> std::exception_ptr {
        assert(is_ready());
        return _p->_exception;
    }
};

template <typename Sig, typename E, typename F>
auto package(E executor, F&& f)
    -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>> {
    if constexpr (std::is_same_v<E, executor_t>) {
        assert(executor && "FATAL (sean.parent) : executor is null!");
    }

    using result_t = detail::result_of_t_<Sig>;

    if constexpr (detail::is_future_v<result_t>) {
        auto p =
            std::make_shared<detail::shared<detail::reduced_signature_t<Sig>>>(std::move(executor));

        p->_f = [_f = std::forward<F>(f)](auto&& promise, auto&&... args) mutable noexcept {
            if (promise.canceled()) return;

            try {
                auto r = std::move(_f)(std::forward<decltype(args)>(args)...);
                try {
                    std::move(r).detach([_p = std::move(promise)](auto&& f) mutable noexcept {
                        if (auto e = f.exception()) {
                            std::move(_p).set_exception(std::move(e));
                        } else {
                            std::move(_p).set_value(invoke_void_to_monostate_result(
                                [&] { return std::forward<decltype(f)>(f).get_ready(); }));
                        }
                    });
                } catch (...) {
                    /* NOTE: an exception here is reported as a broken promise. Ideally detach
                     * would be passed the initial promise (it flows through the chain), but the
                     * API isn't there yet. */
                }
            } catch (...) {
                std::move(promise).set_exception(std::current_exception());
            }
        };
        return {detail::packaged_task_from_signature_t<Sig>{p}, result_t{p}};
    } else {
        auto p = std::make_shared<detail::shared<Sig>>(std::move(executor));

        p->_f = [_f = std::forward<F>(f)](auto&& promise, auto&&... args) mutable noexcept {
            if (promise.canceled()) return;

            try {
                auto tmp = invoke_void_to_monostate_result(std::move(_f),
                                                           std::forward<decltype(args)>(args)...);
                std::move(promise).set_value(std::move(tmp)); // noexcept
            } catch (...) {
                std::move(promise).set_exception(std::current_exception());
            }
        };
        return {detail::packaged_task_from_signature_t<Sig>{p}, future<result_t>{p}};
    }
}

template <class T, class E>
auto future_with_broken_promise(E executor) -> detail::reduced_t<T> {
    auto p =
        std::make_shared<detail::shared<detail::reduced_signature_t<T()>>>(std::move(executor));
    p->_exception = std::make_exception_ptr(future_error(future_error_codes::broken_promise));
    p->_ready = true;

    return detail::reduced_t<T>{p};
}

/**************************************************************************************************/

namespace detail {

template <typename F>
struct assign_ready_future {
    template <typename T>
    static void assign(T& x, F f) {
        x = std::move(*(std::move(f).get_try()));
    }
};

template <>
struct assign_ready_future<future<void>> {
    template <typename T>
    static void assign(T& x, const future<void>&) {
        x = std::move(typename T::value_type()); // to set the optional
    }
};

template <typename F, typename Args>
struct when_all_shared {
    // decay
    Args _args;
    std::mutex _guard;
    std::array<future<void>, std::tuple_size_v<Args>> _holds;
    std::size_t _remaining{std::tuple_size<Args>::value};
    std::exception_ptr _exception;
    packaged_task<> _f;

    template <std::size_t index, typename FF>
    void done(FF&& f) {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{_guard};
            if (!_exception) {
                assign_ready_future<FF>::assign(std::get<index>(_args), std::forward<FF>(f));
                if (--_remaining == 0) run = true;
            }
        }
        if (run) _f();
    }

    void failure(const std::exception_ptr& error) {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{_guard};
            if (!_exception) {
                for (auto& h : _holds)
                    h.reset();
                _exception = error;
                run = true;
            }
        }
        if (run) _f();
    }
};

template <size_t S, typename R>
struct when_any_shared {
    using result_type = R;
    // decay
    std::optional<R> _arg;
    std::mutex _guard;
    std::array<future<void>, S> _holds;
    std::size_t _remaining{S};
    std::exception_ptr _exception;
    std::size_t _index = std::numeric_limits<std::size_t>::max();
    packaged_task<> _f;

    void failure(const std::exception_ptr& error) {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{_guard};
            if (--_remaining == 0) {
                _exception = error;
                run = true;
            }
        }
        if (run) _f();
    }

    template <size_t index, typename FF>
    void done(FF&& f) {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{_guard};
            if (_index == std::numeric_limits<std::size_t>::max()) {
                _arg = std::move(*std::forward<FF>(f).get_try());
                _index = index;
                run = true;
            }
        }
        if (run) _f();
    }

    template <typename F>
    auto apply(F& f) {
        return f(std::move(*_arg), _index);
    }
};

template <size_t S>
struct when_any_shared<S, void> {
    using result_type = void;
    // decay
    std::mutex _guard;
    std::array<future<void>, S> _holds;
    std::size_t _remaining{S};
    std::exception_ptr _exception;
    std::size_t _index = std::numeric_limits<std::size_t>::max();
    packaged_task<> _f;

    void failure(const std::exception_ptr& error) {
        auto run{false};
        std::unique_lock<std::mutex> lock{_guard};
        {
            if (--_remaining == 0) {
                _exception = error;
                run = true;
            }
        }
        if (run) _f();
    }

    template <size_t index, typename FF>
    void done(FF&&) {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{_guard};
            if (_index == std::numeric_limits<std::size_t>::max()) {
                _index = index;
                run = true;
            }
        }
        if (run) _f();
    }

    template <typename F>
    auto apply(F& f) {
        return f(_index);
    }
};

inline void rethrow_if_false(bool x, std::exception_ptr& p) {
    if (!x) std::rethrow_exception(p);
}

template <typename F, typename Args, typename P, std::size_t... I>
auto apply_when_all_args_(F& f, Args& args, P& p, std::index_sequence<I...>) {
    (void)std::initializer_list<int>{
        (rethrow_if_false(static_cast<bool>(std::get<I>(args)), p->_exception), 0)...};
    return apply_optional_indexed<
        index_sequence_transform_t<std::make_index_sequence<std::tuple_size<Args>::value>,
                                   remove_placeholder<Args>::template function>>(f, args);
}

template <typename F, typename P>
auto apply_when_all_args(F& f, P& p) {
    return apply_when_all_args_(
        f, p->_args, p, std::make_index_sequence<std::tuple_size<decltype(p->_args)>::value>());
}

template <typename F, typename P>
auto apply_when_any_arg(F& f, P& p) {
    if (p->_exception) {
        std::rethrow_exception(p->_exception);
    }

    return p->apply(f);
}

template <std::size_t i, typename E, typename P, typename T>
void attach_when_arg_(E&& executor, std::shared_ptr<P>& shared, T a) {
    auto holds =
        std::move(a).recover(std::forward<E>(executor), [_w = std::weak_ptr<P>(shared)](auto x) {
            auto p = _w.lock();
            if (!p) return;

            if (auto ex = x.exception()) {
                p->failure(ex);
            } else {
                p->template done<i>(std::move(x));
            }
        });
    std::unique_lock<std::mutex> lock{shared->_guard};
    shared->_holds[i] = std::move(holds);
}

template <typename E, typename P, typename... Ts, std::size_t... I>
void attach_when_args_(std::index_sequence<I...>, E&& executor, std::shared_ptr<P>& p, Ts... a) {
    (void)std::initializer_list<int>{
        (attach_when_arg_<I>(std::forward<E>(executor), p, std::move(a)), 0)...};
}

template <typename E, typename P, typename... Ts>
void attach_when_args(E&& executor, std::shared_ptr<P>& p, Ts... a) {
    attach_when_args_(std::make_index_sequence<sizeof...(Ts)>(), std::forward<E>(executor), p,
                      std::move(a)...);
}

} // namespace detail

/**************************************************************************************************/

template <typename E, typename F, typename... Ts>
auto when_all(E executor, F f, future<Ts>... args) {
    using vt_t = voidless_tuple<Ts...>;
    using opt_t = optional_placeholder_tuple<Ts...>;
    using result_t = decltype(apply_ignore_placeholders(std::declval<F>(), std::declval<vt_t>()));

    auto shared = std::make_shared<detail::when_all_shared<F, opt_t>>();
    auto p = package<result_t()>(
        executor, [_f = std::move(f), _p = shared] { return detail::apply_when_all_args(_f, _p); });
    shared->_f = std::move(p.first);

    detail::attach_when_args(executor, shared, std::move(args)...);

    return std::move(p.second);
}

/**************************************************************************************************/

template <typename T>
struct make_when_any {
    template <typename E, typename F, typename... Ts>
    static auto make(E executor, F f, future<T> arg, future<Ts>... args) {
        using result_t = detail::result_t<F, T, size_t>;

        auto shared = std::make_shared<detail::when_any_shared<sizeof...(Ts) + 1, T>>();
        auto p = package<result_t()>(executor, [_f = std::move(f), _p = shared]() mutable {
            return detail::apply_when_any_arg(_f, _p);
        });
        shared->_f = std::move(p.first);

        detail::attach_when_args(executor, shared, std::move(arg), std::move(args)...);

        return std::move(p.second);
    }
};

/**************************************************************************************************/

template <>
struct make_when_any<void> {
    template <typename E, typename F, typename... Ts>
    static auto make(E executor, F&& f, future<Ts>... args) {
        using result_t = detail::result_t<F, size_t>;

        auto shared = std::make_shared<detail::when_any_shared<sizeof...(Ts), void>>();
        auto p = package<result_t()>(executor, [_f = std::forward<F>(f), _p = shared]() mutable {
            return detail::apply_when_any_arg(_f, _p);
        });
        shared->_f = std::move(p.first);

        detail::attach_when_args(executor, shared, std::move(args)...);

        return std::move(p.second);
    }
};

/**************************************************************************************************/

template <typename E, typename F, typename T, typename... Ts>
auto when_any(E executor, F&& f, future<T> arg, future<Ts>... args) {
    return make_when_any<T>::make(std::move(executor), std::forward<F>(f), std::move(arg),
                                  std::move(args)...);
}

/**************************************************************************************************/

namespace detail {
template <typename T>
struct value_storer {
    template <typename C, typename F>
    static void store(C& context, F&& f, std::size_t index) {
        context._results = std::move(*std::forward<F>(f).get_try());
        context._index = index;
    }
};

template <typename T>
struct value_storer<std::vector<T>> {
    template <typename C, typename F>
    static void store(C& context, F&& f, std::size_t index) {
        context._results[index] = std::move(*std::forward<F>(f).get_try());
    }
};

template <bool Indexed, typename R>
struct result_creator;

template <>
struct result_creator<true, void> {
    template <typename C>
    static auto go(C& context) {
        return context._f(context._index);
    }
};

template <>
struct result_creator<false, void> {
    template <typename C>
    static auto go(C& context) {
        return context._f();
    }
};

template <typename R>
struct result_creator<true, R> {
    template <typename C>
    static auto go(C& context) {
        return context._f(std::move(context._results), context._index);
    }
};

template <typename R>
struct result_creator<false, R> {
    template <typename C>
    static auto go(C& context) {
        return context._f(std::move(context._results));
    }
};

template <typename F, bool Indexed, typename R>
struct context_result {
    using result_type = R;

    R _results;
    std::exception_ptr _exception;
    std::size_t _index{0};
    F _f;

    context_result(F f, std::size_t s) : _f(std::move(f)) { init(_results, s); }

    template <typename T>
    void init(std::vector<T>& v, std::size_t s) {
        v.resize(s);
    }

    template <typename T>
    void init(T&, std::size_t) {}

    template <typename FF>
    void apply(FF&& f, std::size_t index) {
        value_storer<R>::store(*this, std::forward<FF>(f), index);
    }

    void apply(const std::exception_ptr& error, std::size_t) { _exception = error; }

    auto operator()() { return result_creator<Indexed, R>::go(*this); }
};

template <typename F, bool Indexed>
struct context_result<F, Indexed, void> {
    std::exception_ptr _exception;
    std::size_t _index{0};
    F _f;

    context_result(F f, std::size_t) : _f(std::move(f)) {}

    template <typename FF>
    void apply(FF&&, std::size_t index) {
        _index = index;
    }

    void apply(const std::exception_ptr& error, std::size_t) { _exception = error; }

    auto operator()() { return result_creator<Indexed, void>::go(*this); }
};

/**************************************************************************************************/

/*
 * This specialization is used for cases when only one ready future is enough to move forward.
 * In case of when_any, the first successful future triggers the continuation. All others are
 * cancelled. In case of when_all, after the first error, this future cannot be fulfilled
 * anymore and so we cancel the all the others.
 */
struct single_trigger {
    template <typename C, typename F>
    static auto go(C& context, F&& f, size_t index) -> bool {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{context._guard};
            if (!context._single_event) {
                for (auto i = 0u; i < context._holds.size(); ++i) {
                    if (i != index) context._holds[i].reset();
                }
                context._single_event = true;
                context.apply(std::forward<F>(f), index);
                run = true;
            }
        }
        return run;
    }
};

/*
 * This specialization is used for cases when all futures must be fulfilled before the
 * continuation is triggered. In case of when_any it means, that the error case handling is
 * started, because all futures failed. In case of when_all it means, that after all futures
 * were fulfilled, the continuation is started.
 */
struct all_trigger {
    template <typename C, typename F>
    static auto go(C& context, F&& f, size_t index) -> bool {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{context._guard};
            context.apply(std::forward<F>(f), index);
            if (--context._remaining == 0) run = true;
        }
        return run;
    }

    template <typename C>
    static auto go(C& context, const std::exception_ptr& error, size_t index) -> bool {
        auto run{false};
        {
            std::unique_lock<std::mutex> lock{context._guard};
            if (--context._remaining == 0) {
                context.apply(error, index);
                run = true;
            }
        }
        return run;
    }
};

template <typename CR, typename F, typename ResultCollector, typename FailureCollector>
struct common_context : CR {
    std::mutex _guard;
    std::size_t _remaining;
    bool _single_event{false};
    std::vector<future<void>> _holds;
    packaged_task<> _f;

    common_context(F f, size_t s) : CR(std::move(f), s), _remaining(s), _holds(_remaining) {}

    auto execute() {
        if (this->_exception) {
            std::rethrow_exception(this->_exception);
        }
        return CR::operator()();
    }

    void failure(const std::exception_ptr& error, size_t index) {
        if (FailureCollector::go(*this, error, index)) _f();
    }

    template <typename FF>
    void done(FF&& f, size_t index) {
        if (ResultCollector::go(*this, std::forward<FF>(f), index)) _f();
    }
};

/**************************************************************************************************/

template <typename C, typename E, typename T>
void attach_tasks(size_t index, E executor, const std::shared_ptr<C>& context, T&& a) {
    auto&& hold = std::forward<T>(a).recover(
        std::move(executor), [_context = make_weak_ptr(context), _i = index](auto x) {
            auto p = _context.lock();
            if (!p) return;
            if (auto ex = x.exception()) {
                p->failure(ex, _i);
            } else {
                p->done(std::move(x), _i);
            }
        });

    std::unique_lock<std::mutex> guard(context->_guard);
    context->_holds[index] = std::move(hold);
}

template <typename R, typename T, typename C, typename Enabled = void>
struct create_range_of_futures;

template <typename R, typename T, typename C>
struct create_range_of_futures<R, T, C, enable_if_copyable<T>> {
    template <typename E, typename F, typename I>
    static auto do_it(E executor, F&& f, I first, I last) {
        assert(first != last);

        auto context = std::make_shared<C>(std::forward<F>(f), std::distance(first, last));
        auto p = package<R()>(executor, [_c = context]() mutable { return _c->execute(); });

        context->_f = std::move(p.first);

        size_t index(0);
        for (; first != last; ++first) {
            attach_tasks(index++, executor, context, *first);
        }

        return std::move(p.second);
    }
};

template <typename R, typename T, typename C>
struct create_range_of_futures<R, T, C, enable_if_not_copyable<T>> {
    template <typename E, typename F, typename I>
    static auto do_it(E executor, F&& f, I first, I last) {
        assert(first != last);

        auto context = std::make_shared<C>(std::forward<F>(f), std::distance(first, last));
        auto p = package<R()>(executor, [_c = context] { return _c->execute(); });

        context->_f = std::move(p.first);

        size_t index(0);
        for (; first != last; ++first) {
            attach_tasks(index++, executor, context, std::move(*first));
        }

        return std::move(p.second);
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename E, // models task executor
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures of the same
                      // type
auto when_all(E executor, F f, std::pair<I, I> range) {
    using param_t = typename std::iterator_traits<I>::value_type::result_type;
    using result_t = typename detail::result_of_when_all_t<F, param_t>::result_type;
    using context_result_t =
        std::conditional_t<std::is_same_v<void, param_t>, void, std::vector<param_t>>;
    using context_t = detail::common_context<detail::context_result<F, false, context_result_t>, F,
                                             detail::all_trigger, detail::single_trigger>;

    if (range.first == range.second) {
        auto p = package<result_t()>(
            executor, detail::context_result<F, false, context_result_t>(std::move(f), 0));
        executor(std::move(p.first));
        return std::move(p.second);
    }

    return detail::create_range_of_futures<result_t, param_t, context_t>::do_it(
        executor, std::move(f), range.first, range.second);
}

/**************************************************************************************************/

template <typename E, // models task executor
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures of the same
                      // type
auto when_any(E executor, F&& f, std::pair<I, I> range) {
    using param_t = typename std::iterator_traits<I>::value_type::result_type;
    using result_t = typename detail::result_of_when_any_t<F, param_t>::result_type;
    using context_result_t = std::conditional_t<std::is_same_v<void, param_t>, void, param_t>;
    using context_t = detail::common_context<detail::context_result<F, true, context_result_t>, F,
                                             detail::single_trigger, detail::all_trigger>;

    if (range.first == range.second) {
        return future_with_broken_promise<result_t>(executor);
    }

    return detail::create_range_of_futures<result_t, param_t, context_t>::do_it(
        std::move(executor), std::forward<F>(f), range.first, range.second);
}

/**************************************************************************************************/

#if 0
        std::bind<result_type>(
            [_f = std::forward<F>(f)](
                unwrap_reference_t<std::decay_t<Args>>&... brgs) mutable -> result_type {
                return std::move(_f)(move_if<!is_reference_wrapper_v<std::decay_t<Args>>>(brgs)...);
            },
            std::forward<Args>(args)...));
#endif

template <typename E, typename F, typename... Args>
auto async(E executor, F&& f, Args&&... args)
    -> detail::reduced_t<detail::result_t<std::decay_t<F>, std::decay_t<Args>...>> {
    using result_type = detail::result_t<std::decay_t<F>, std::decay_t<Args>...>;

    auto [pro, fut] = package<result_type()>(
        executor,
        [f = std::forward<F>(f), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
        -> result_type { return std::apply(std::move(f), std::move(args)); });

    executor(std::move(pro));

    return std::move(fut);
}

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <class T>
struct value_<T, enable_if_copyable<void_to_monostate_t<T>>> {
    template <class C>
    static void proceed(C& sb) {
        typename C::then_t then;
        {
            std::unique_lock<std::mutex> lock(sb._mutex);
            sb._ready = true;
            then = std::move(sb._then);
        }
        for (auto& e : then)
            e.first(std::move(e.second));
    }

    template <class R, class A>
    static void set(shared_base<R>& sb, A&& a) {
        sb._result = std::forward<A>(a);
        proceed(sb);
    }
};

template <typename T>
struct value_<T, enable_if_not_copyable<void_to_monostate_t<T>>> {
    template <class C>
    static void proceed(C& sb) {
        typename C::then_t then;
        {
            std::unique_lock<std::mutex> lock(sb._mutex);
            sb._ready = true;
            then = std::move(sb._then);
        }
        if (then.first) then.first(std::move(then.second));
    }

    template <class R, class A>
    static void set(shared_base<R>& sb, A&& a) {
        sb._result = std::forward<A>(a);
        proceed(sb);
    }
};

/**************************************************************************************************/

template <class T>
template <class A>
void shared_base<T, enable_if_copyable<void_to_monostate_t<T>>>::set_value(A&& a) {
    value_<T>::set(*this, std::forward<A>(a));
}

template <typename T>
template <class A>
void shared_base<T, enable_if_not_copyable<void_to_monostate_t<T>>>::set_value(A&& a) {
    value_<T>::set(*this, std::forward<A>(a));
}

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

} // namespace STLAB_VERSION_NAMESPACE()
} // namespace stlab

/**************************************************************************************************/

#ifndef STLAB_NO_STD_COROUTINES

template <typename T, typename... Args>
struct std::coroutine_traits<stlab::future<T>, Args...> {
    struct promise_type {
        std::pair<stlab::packaged_task<T>, stlab::future<T>> _promise;

        promise_type() {
            _promise = stlab::package<T(T)>(stlab::immediate_executor, std::identity{});
        }

        stlab::future<T> get_return_object() { return std::move(_promise.second); }

        auto initial_suspend() const { return std::suspend_never{}; }

        auto final_suspend() const noexcept { return std::suspend_never{}; }

        template <typename U>
        void return_value(U&& val) {
            _promise.first(std::forward<U>(val));
        }

        void unhandled_exception() { _promise.first.set_exception(std::current_exception()); }
    };
};

template <typename... Args>
struct std::coroutine_traits<stlab::future<void>, Args...> {
    struct promise_type {
        std::pair<stlab::packaged_task<>, stlab::future<void>> _promise;

        inline promise_type() {
            _promise = stlab::package<void()>(stlab::immediate_executor, []() {});
        }

        inline stlab::future<void> get_return_object() { return _promise.second; }

        inline auto initial_suspend() const { return std::suspend_never{}; }

        inline auto final_suspend() const noexcept { return std::suspend_never{}; }

        inline void return_void() { _promise.first(); }

        inline void unhandled_exception() {
            _promise.first.set_exception(std::current_exception());
        }
    };
};

template <typename R>
auto operator co_await(stlab::future<R> f) {
    struct Awaiter {
        stlab::future<R> _input;

        bool await_ready() { return _input.is_ready(); }

        auto await_resume() { return std::move(_input).get_ready(); }

        void await_suspend(std::coroutine_handle<> ch) {
            std::move(_input).detach([this, ch](stlab::future<R>&& f) {
                _input = std::move(f);
                ch.resume();
            });
        }
    };
    return Awaiter{std::move(f)};
}

#endif // STLAB_NO_STD_COROUTINES

#endif
