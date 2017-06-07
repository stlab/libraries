#include <iostream>

#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;

int main(int argc, const char * argv[]) {
    serial_queue_t cout_queue(stlab::default_executor);
    serial_queue_t cerr_queue(stlab::default_executor);

    cout_queue([](){
        std::cout << "a1      (1)\n";
    });

    cerr_queue([](){
        std::cerr << "   b1   (2)\n";
    });

    cerr_queue([](){
        std::cerr << "   b2   (3)\n";
    }).then([](){
        std::cerr << "   b2.1 (3.1)\n";
    });

    cerr_queue([](){
        std::cerr << "   b3   (4)\n";
    });

    cout_queue([](){
        std::cout << "a2      (5)\n";
    });

    cout_queue([](){
        std::cout << "a3      (6)\n";
    });

    return 0;
}
