#include <cstdio>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto x = async(default_executor, [] { return 42; });

    auto c1 = x.then([](int x) { printf("Split A %d \n", x); });
    auto c2 = x.then([](int x) { printf("Split B %d \n", x); });

    // Waiting just for illustration purpose
    while (!c1.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
    while (!c2.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    pre_exit();
}
/*
    Result: (Note: The order of the results is not defined.)

        Split B 42
        Split A 42
*/
