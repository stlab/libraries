/*
    Copyright 2022 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_PRE_EXIT_HPP
#define STLAB_PRE_EXIT_HPP

#include <stlab/config.hpp>

/**************************************************************************************************/

// The namespace for pre_exit cannot be changed without an ABI break. If making an ABI breaking
// change in this file it needs to be done in a way supporting this version as well.
namespace stlab {
inline namespace STLAB_VERSION_NAMESPACE() {

/**************************************************************************************************/

/// Pre-exit handler type.
#if __cpp_noexcept_function_type >= 201510L
using pre_exit_handler = void (*)() noexcept;
#else
using pre_exit_handler = void (*)();
#endif

/// Invoke all registered pre-exit handlers in the reverse order they are registered. It is safe
/// to register additional handlers during this operation. Must be invoked exactly once prior to
/// program exit.
extern "C" void pre_exit();

/// Register a pre-exit handler. The pre-exit-handler may not throw. With C++17 or later it
/// is required to be `noexcept`.
extern "C" void at_pre_exit(pre_exit_handler f);

/**************************************************************************************************/

} // namespace v2
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
