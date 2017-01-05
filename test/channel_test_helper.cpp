#include "channel_test_helper.hpp"

std::queue<std::function<void()>> manual_scheduler::_tasks;
std::atomic_int timed_sum::_x{ 0 };