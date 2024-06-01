#include <cstdio>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto x = async(default_executor, [] { return 42; });

    auto y = x.then([](int x) { printf("Result %d \n", x); });

    // Waiting just for illustration purpose
    while (!y.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    pre_exit();
}

// Result: Result 42
