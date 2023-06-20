#include "bindings.h"

#include <iostream>
#include <type_traits>
#include <utility>

namespace stlab {
inline namespace v1 {
namespace detail {

// REVISIT - have to invert the priority encoding to match old API.
enum class executor_priority { high = 2, medium = 1, low = 0 };

/// @brief Asynchronously invoke f on the default_executor.
/// @tparam F function object type
/// @param f a function object.
/// @return the result of calling `execute`.
template <class F, typename = std::enable_if_t<std::is_invocable_r<void, F>::value>>
auto enqueue(F f) {
    return execute(new F(std::move(f)), [](void* f_) {
        auto f = static_cast<F*>(f_);
        try {
            (*f)();
        } catch (...) {}
        delete f;
    });
}

/// @brief Asynchronously invoke `f` on the default_executor with priority `p`.
/// @tparam F function object type
/// @param f a function object.
/// @param priority 
/// @return the value returned by `execute_priority`.
template <class F, class = std::enable_if_t<std::is_invocable_r< void, F >::value> >
auto enqueue_priority(F f, executor_priority p) {
    return execute_priority(new F(std::move(f)), [](void* f_) {
        auto f = static_cast<F*>(f_);
        try {
            (*f)();
        } catch (...) {}
        delete f;
    }, static_cast<std::uint64_t>(p));
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
