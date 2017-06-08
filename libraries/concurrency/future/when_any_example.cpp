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

    auto result = when_all(default_executor, [](int x) {
        cout << "The current result is " << x '\n';
    }, argument1, argument2);

    // Waiting just for illustrational purpose
    while (!result.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
}

/*
    Result:

        The current result is 42
        or 
        The current result is 815
*/
