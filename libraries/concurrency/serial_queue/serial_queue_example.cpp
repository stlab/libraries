#include <iostream>
#include <thread>
#include <string>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>

using namespace std;
using namespace stlab;

serial_queue_t io_q(default_executor);
atomic<bool>   done{false};

void say_hello(string to_whom) {
    io_q.executor()([_whom = move(to_whom)](){
        cout << "Hello, " << _whom << "!\n";
    });
}

int main(int, char**) {
    say_hello("fella");
    say_hello("chief");
    say_hello("pal");
    say_hello("world");
    auto f = io_q([&_done = done](){
        _done = true;
    });

    while (!f.get_try()) { }
}

/*
    Result:

        Hello, fella!
        Hello, chief!
        Hello, pal!
        Hello, world!
*/
