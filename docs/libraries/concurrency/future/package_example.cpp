#include <iostream>
#include <string>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto p = package<int(int)>(default_executor, [](int x) { return x+x; });
    auto packagedTask = p.first;
    auto f = p.second;

    packagedTask(21);

    // Waiting just for illustration purpose
    while (!f.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    cout << "The answer is " << *f.get_try() << "\n";

    pre_exit();
}

/*
    Result:

        The answer is 42
*/
