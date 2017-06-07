#include <iostream>

#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;

int main(int argc, const char * argv[]) {
    serial_queue_t aq(stlab::default_executor);
    serial_queue_t bq(stlab::default_executor);
    serial_queue_t cq(stlab::default_executor);
    serial_queue_t dq(stlab::default_executor);

    aq([](){
        std::cout << "a1                      ( 1)\n";
    });

    bq([](){
        std::cout << "   b1                   ( 2)\n";
    });

    dq([](){
        std::cout << "           d1           ( 3)\n";
    });

    bq([](){
        std::cout << "   b2                   ( 4)\n";
    }).then([](){
        std::cout << "   b2.1                 ( 4.1)\n";
    }).detach();

    cq([](){
        std::cout << "        c1              ( 5)\n";
    });

    cq([](){
        std::cout << "        c2              ( 6)\n";
    });

    dq([](){
        std::cout << "           d2           ( 7)\n";
    });

    cq([](){
        std::cout << "        c3              ( 8)\n";
    });

    bq([](){
        std::cout << "   b3                   ( 9)\n";
    });

    aq([](){
        std::cout << "a2                      (10)\n";
    });

    aq([](){
        std::cout << "a3                      (11)\n";
    });

    dq([](){
        std::cout << "           d3           (12)\n";
    });

    return 0;
}
