//
// Created by Felix Petriconi on 07/01/17.
//

#ifndef SLABFUTURE_EXECUTOR_BASE_HPP
#define SLABFUTURE_EXECUTOR_BASE_HPP

#include <chrono>
#include <boost/variant.hpp>

namespace stlab
{

using executor_t = std::function<void(std::function<void()>)>;
using timed_executor_t = std::function<void(std::chrono::system_clock::time_point, std::function<void()>)>;

#if 0
/*
 * returns an executor that will schedule any passed task to it to execute
 * at the given time point on the executor provided
 * */

auto execute_at(std::chrono::system_clock::time_point when, timed_schedule_t executor)
{
    return detail::executor_wrapper(when, std::move(executor));
}

/*
 * returns an executor that will schedule the task to execute on the provided
 * executor duration after it is invoked
 * */
template <typename E>
auto execute_delayed(std::chrono::system_clock::duration, E executor) {
    return executor;
}

#endif
}

#endif // SLABFUTURE_EXECUTOR_BASE_HPP
