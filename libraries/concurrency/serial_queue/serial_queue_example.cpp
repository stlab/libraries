#include <iostream>
#include <thread>
#include <string>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>

using namespace std;
using namespace stlab;

serial_queue_t io_q(default_executor);

void say_hello(string to_whom) {
    auto f = io_q([_whom = move(to_whom)](){
        cout << "Hello, " << _whom << "!\n";
    });

    while (!f.get_try()) { }
}

int main(int, char**) {
    auto t0 = thread(&say_hello, "fella");
    auto t1 = thread(&say_hello, "chief");
    auto t2 = thread(&say_hello, "pal");
    auto t3 = thread(&say_hello, "world");

    t0.join();
    t1.join();
    t2.join();
    t3.join();
}

/*
    Result: (Order of outputs may change)

        Hello, fella!
        Hello, world!
        Hello, chief!
        Hello, pal!
*/
