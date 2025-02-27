/*
    Copyright 2025 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <stlab/pre_exit.hpp>

#include <cassert>
#include <mutex>
#include <vector>

namespace stlab {
inline namespace v2 {

namespace {

struct pre_exit_stack_t {
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
    auto pop() -> pre_exit_handler {
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

    ~pre_exit_stack_t() {
        assert(_closed && "WARNING: `pre_exit()` not called before program exit.");
    }
};

auto pre_exit_stack() -> auto& {
    static pre_exit_stack_t _q;
    return _q;
}

} // namespace

extern "C" void stlab_pre_exit() {
    auto& _s = pre_exit_stack();
    while (auto f = _s.pop()) {
        f();
    };
}

extern "C" void stlab_at_pre_exit(pre_exit_handler f) { pre_exit_stack().push(f); }

} // namespace v2
} // namespace stlab
