#include <experimental/coroutine>

#define STLAB_WITH_COROUTINE_SUPPORT

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <iostream>
#include <thread>

stlab::future<int> get_the_answer() {
    auto result = co_await stlab::async(stlab::default_executor, 
        [] { 
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
            return 42; 
        });
    
    co_return result;
}

stlab::future<void> wait_for_the_answer() {
    co_await stlab::async(stlab::default_executor,
        [] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });

    co_return;
}


int main()
{
    auto w = get_the_answer();

    while (!w.get_try())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << w.get_try().value() << '\n';

    auto x = wait_for_the_answer();

    while (!x.get_try())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "The answer is there\n";

    return 0;
}