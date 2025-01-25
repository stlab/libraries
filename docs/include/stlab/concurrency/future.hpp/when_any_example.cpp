#include <iostream>
#include <string>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto argument1 = async(default_executor, [] { return 42; });
    auto argument2 = async(default_executor, [] { return 815; });

    auto result = when_any(default_executor, [](int x, std::size_t index) {
        cout << "The current result is " << x << " " << index << '\n';
    }, argument1, argument2);

    // Waiting just for illustration purpose
    while (!result.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    pre_exit();
}

/*
    Result:

        The current result is 42 0
        or
        The current result is 815 1
*/
