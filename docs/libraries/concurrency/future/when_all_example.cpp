#include <iostream>
#include <string>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto argument1 = async(default_executor, [] { return 42; });
    auto argument2 = async(default_executor, [] { return string("The answer is"); });

    auto result = when_all(default_executor, [](int answer, std::string text) {
        cout << text << " " << answer << '\n';
    }, argument1, argument2);

    // Waiting just for illustration purpose
    while (!result.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    pre_exit();
}

/*
    Result:

        The answer is 42
*/
