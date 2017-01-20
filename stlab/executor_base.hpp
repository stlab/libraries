/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SLABFUTURE_EXECUTOR_BASE_HPP
#define SLABFUTURE_EXECUTOR_BASE_HPP

#include <chrono>
#include <functional>
#include <stlab/system_timer.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

using executor_t = std::function<void(std::function<void()>)>;

/*
 * returns an executor that will schedule any passed task to it to execute
 * at the given time point on the executor provided
 */

inline executor_t execute_at(std::chrono::system_clock::time_point when, executor_t executor)
{
    return [=](auto f) {
        if ( (when != std::chrono::system_clock::time_point()) && (when > std::chrono::system_clock::now()) )
            system_timer(when, [=]{
                executor(f);
        });
        else 
            executor(f);
    };
}

/*
 * returns an executor that will schedule the task to execute on the provided
 * executor duration after it is invoked
 */

template<typename E>
auto execute_delayed(std::chrono::system_clock::duration duration, E executor) {
    return execute_at(std::chrono::system_clock::now() + duration, std::move(executor));
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // SLABFUTURE_EXECUTOR_BASE_HPP
