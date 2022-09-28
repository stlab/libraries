
#include <iostream>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    future<int> a = async(default_executor, [] { cout << 1 << "\n"; return 42; }).then([] (auto x) {
        return async(default_executor, [] (auto x) { cout << 2 << "\n"; return x + 42; }, x);
    });

    while (!a.get_try()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    cout << *a.get_try() << '\n';

    pre_exit();
}
/*
    Result:
        1
        2
        84
*/