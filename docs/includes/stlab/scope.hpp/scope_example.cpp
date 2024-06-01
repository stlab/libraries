#include <iostream>
#include <mutex>
#include <thread>
#include <stlab/scope.hpp>

using namespace stlab;
using namespace std;

mutex m;

void scoped() {
    stlab::scope<lock_guard<mutex>>(m, [&](){
        cout << "Hello, world!\n";
    });
}

int main(int, char**) {
    auto t0 = thread(&scoped);
    auto t1 = thread(&scoped);
    auto t2 = thread(&scoped);

    t0.join();
    t1.join();
    t2.join();
}
/*
    Result:

        Hello, world!
        Hello, world!
        Hello, world!
*/
