#include "bindings.h"

#include <utility>
#include <iostream>

namespace stlab {
inline namespace v1 {
namespace detail {

// REVISIT - have to invert the priority encoding to match old API.
enum class executor_priority { high = 2, medium = 1, low = 0 };

/// @brief Invokes `f` on the default_executor.
/// @tparam F function object type
/// @param f a function object with signature `void()`.
/// @return the value returned by `execute`.
template <class F>
auto enqueue(F f) {
    using f_t = decltype(f);
    return execute(new f_t(std::move(f)), [](void* f_) {
        auto f = static_cast<f_t*>(f_);
        (*f)();
        delete f;
    });
}

/// @brief Invokes `f` on the default_executor at the given `priority`.
/// @tparam F function object type
/// @param f a function object with signature `void()`.
/// @param priority 
/// @return the value returned by `execute_priority`.
template <class F>
auto enqueue_priority(F f, executor_priority priority) {
    using f_t = decltype(f);
    return execute_priority(new f_t(std::move(f)), [](void* f_) {
        auto f = static_cast<f_t*>(f_);
        (*f)();
        delete f;
    }, static_cast<std::size_t>(priority));
}

/// @brief A thin invokable wrapper around `enqueue_priority`.
/// @tparam Priority the priority at which all given function objects will be enqueued.
template <executor_priority Priority>
struct executor_type {
    using result_type = void;

    /// @brief Enqueues the given task on the default_executor with this object's Priority value.
    /// @tparam F function object type
    /// @param f function object
    template <class F>
    void operator()(F&& f) const {
        enqueue_priority(std::forward<F>(f), Priority);
    }
};

} // namespace detail

/// @brief An executor for low priority tasks, enqueued with the call operator.
constexpr auto low_executor = detail::executor_type<detail::executor_priority::low>{};
/// @brief An executor for standard priority tasks, enqueued with the call operator.
constexpr auto default_executor = detail::executor_type<detail::executor_priority::medium>{};
/// @brief An executor for high priority tasks, enqueued with the call operator.
constexpr auto high_executor = detail::executor_type<detail::executor_priority::high>{};

} // inline namespace v1
} // namespace stlab
