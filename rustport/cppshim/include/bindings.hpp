#include "bindings.h"

#include <iostream>
#include <type_traits>
#include <utility>

namespace rust {

/// @brief Asynchronously invoke f on the rust-backed executor.
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

/// @brief Asynchronously invoke `f` on the rust-backed executor with priority `p`.
/// @tparam F function object type
/// @param f a function object.
/// @param priority 
/// @return the value returned by `execute_priority`.
template <class F, class = std::enable_if_t<std::is_invocable_r< void, F >::value> >
auto enqueue_priority(F f, Priority p) {
    return execute_priority(new F(std::move(f)), [](void* f_) {
        auto f = static_cast<F*>(f_);
        try {
            (*f)();
        } catch (...) {}
        delete f;
    }, p);
}

}
