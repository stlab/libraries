#include <iostream>
#include <mutex>
#include <thread>

#include <stlab/scope.hpp>
#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;
using namespace std;

int main(int, char**) {
    auto e = stlab::scope<int>(0, []{
        serial_queue_t q(default_executor);
        return q.executor();
    });

    atomic<int> c{0};

    auto t0 = std::thread([&]{e([&]{ cout << "Hello" << ", " << "foo!" << '\n'; ++c; });});
    auto t1 = std::thread([&]{e([&]{ cout << "Hello" << ", " << "bar!" << '\n'; ++c; });});
    auto t2 = std::thread([&]{e([&]{ cout << "Hello" << ", " << "baz!" << '\n'; ++c; });});

    t0.join();
    t1.join();
    t2.join();

    while (c != 3);
}
/*
    Result:

        Hello, baz!
        Hello, foo!
        Hello, bar!

    (The order will be arbitrary, but the output expressions will not be broken up.)
*/
