#include <iostream>
#include <string>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto fv = []{
        return stlab::make_ready_future();
    };
    auto fi = []{
        return stlab::make_ready_future<int>(42);
    };
    auto fs = []{
        return stlab::make_ready_future<std::string>("Hello, world!");
    };
    auto fb = []{
        return stlab::make_ready_future<bool>(true);
    };

    auto f = when_all(stlab::default_executor, [](auto... args){
        for_each_argument([](auto x) { cout << x << "\n"; }, args...);
    }, fi(), fv(), fs(), fv(), fb(), fv());

    while (!f.get_try()) std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/*
    Result:

        42
        Hello, world!
        1
*/
