/*
    Copyright 2022 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_PRE_EXIT_HPP
#define STLAB_PRE_EXIT_HPP

/**************************************************************************************************/

#include <cassert>
#include <mutex>
#include <vector>

/**************************************************************************************************/

namespace stlab {
inline namespace v1 {

/// Pre-exit handler type.
#if __cpp_noexcept_function_type >= 201510L
using pre_exit_handler = void (*)() noexcept;
#else
using pre_exit_handler = void (*)();
#endif

namespace detail {

/// Thread-safe stack of pre-exit handlers. The stack closes after the last element is popped.
struct _pre_exit_stack_t {
    using lock_t = std::unique_lock<std::mutex>;

    std::mutex _mutex;
    std::vector<pre_exit_handler> _stack;
    bool _closed{false};

    /// Push an exit handler. Precondition that stack is not closed.
    void push(pre_exit_handler f) {
        lock_t lock{_mutex};
        assert(
            !_closed &&
            "WARNING: Adding pre-exit handler with `at_pre_exit()` after `pre_exit()` completed.");
        _stack.push_back(f);
    }

    /// Pop one exit handler, returns `nullptr` and closes stack if empty.
    pre_exit_handler pop() {
        lock_t lock{_mutex};
        if (_stack.empty()) {
            assert(!_closed && "WARNING `pre_exit()` invoked more than once.");
            _closed = true;
            return nullptr;
        }
        auto result = _stack.back();
        _stack.pop_back();
        return result;
    }

    ~_pre_exit_stack_t() {
        assert(_closed && "WARNING: `pre_exit()` not called before program exit.");
    }
};

/// Returns a reference to the pre-exit stack singleton.
inline auto& _pre_exit_stack() {
    static _pre_exit_stack_t _q;
    return _q;
}

} // namespace detail

/// Invoke all registered pre-exit handlers in the reverse order they are registered. It is safe
/// to register additional handlers during this operation. Must be invoked exactly once prior to
/// program exit.
inline void pre_exit() {
    auto& _s = detail::_pre_exit_stack();
    while (auto f = _s.pop()) {
        f();
    };
}

/// Register a pre-exit handler. The pre-exit-handler may not throw. With C++17 or later it
/// is required to be `noexcept`.
inline void at_pre_exit(pre_exit_handler f) { detail::_pre_exit_stack().push(f); }

} // namespace v1
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
