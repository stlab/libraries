/*
    Copyright 2025 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <stlab/pre_exit.hpp>

namespace stlab {
inline namespace v1 {
namespace detail {

_pre_exit_stack_t& _pre_exit_stack() {
    static _pre_exit_stack_t _q;
    return _q;
}

} // namespace detail
} // namespace v1
} // namespace stlab
