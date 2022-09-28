#include <iostream>
#include <thread>
#include <string>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>

using namespace std;
using namespace stlab;

serial_queue_t ioq(default_executor);

void say_hello(string to_whom) {
    ioq.executor()([_whom = move(to_whom)](){
        cout << "Hello, " << _whom << "!\n";
    });
}

int main(int, char**) {
    say_hello("fella");
    say_hello("chief");
    say_hello("pal");
    say_hello("world");

    auto done = ioq([]{});

    while (!done.get_try()) { }

    pre_exit();
}

/*
    Result:

        Hello, fella!
        Hello, chief!
        Hello, pal!
        Hello, world!
*/
