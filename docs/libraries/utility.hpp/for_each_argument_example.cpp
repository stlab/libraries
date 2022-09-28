#include <iostream>
#include <mutex>
#include <thread>

#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/utility.hpp>

using namespace stlab;
using namespace std;

int main(int, char**) {
    auto fi = []{
        static std::size_t count_s{0};
        return make_ready_future<int>(count_s++, immediate_executor);
    };

    auto f = when_all(default_executor, [](auto... args){
        for_each_argument([](auto x) { cout << x << "\n"; }, args...);
    }, fi(), fi(), fi());

    while (!f.get_try()) this_thread::sleep_for(chrono::milliseconds(1));

    pre_exit();
}
/*
    Result:

        0
        1
        2
    or
        2
        1
        0
*/
