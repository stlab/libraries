#include <iostream>
#include <string>

#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>

/**************************************************************************************************/

using namespace stlab;

/**************************************************************************************************/

void test0() {
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
}

/**************************************************************************************************/

std::uint64_t str_hash(const std::string& x) {
    std::uint64_t result(0xcbf29ce484222325);

    for (unsigned char c : x)
        result = (result ^ c) * 0x100000001b3;

    return result;
}

/**************************************************************************************************/

inline std::uint64_t hash_combine(std::uint64_t hash, const std::string& x) {
    return hash ^ (str_hash(x) << 1);
};

/**************************************************************************************************/

void test1() {
    serial_queue_t aq(stlab::default_executor);
    serial_queue_t bq(stlab::default_executor);
    serial_queue_t cq(stlab::default_executor);
    serial_queue_t dq(stlab::default_executor);
    std::uint64_t  ahash(str_hash("a"));
    std::uint64_t  bhash(str_hash("b"));
    std::uint64_t  chash(str_hash("c"));
    std::uint64_t  dhash(str_hash("d"));

    aq([&ahash](){
        ahash = hash_combine(ahash, "1");
    });

    bq([&bhash](){
        bhash = hash_combine(bhash, "1");
    });

    dq([&dhash](){
        dhash = hash_combine(dhash, "1");
    });

    bq([&bhash](){
        bhash = hash_combine(bhash, "2");
    });

    cq([&chash](){
        chash = hash_combine(chash, "1");
    });

    cq([&chash](){
        chash = hash_combine(chash, "2");
    });

    dq([&dhash](){
        dhash = hash_combine(dhash, "2");
    });

    cq([&chash](){
        chash = hash_combine(chash, "3");
    });

    bq([&bhash](){
        bhash = hash_combine(bhash, "3");
    });

    aq([&ahash](){
        ahash = hash_combine(ahash, "2");
    });

    aq([&ahash](){
        ahash = hash_combine(ahash, "3");
    });

    dq([&dhash](){
        dhash = hash_combine(dhash, "3");
    });

    std::cout << "a hash " << (ahash == 0xF1A486D58A02A59AUL ? "OK" : "BAD") << " (" << ahash << ")\n";
    std::cout << "b hash " << (bhash == 0xF1A485D58A02B8B3UL ? "OK" : "BAD") << " (" << bhash << ")\n";
    std::cout << "c hash " << (chash == 0xF1A484D58A02A6E4UL ? "OK" : "BAD") << " (" << chash << ")\n";
    std::cout << "d hash " << (dhash == 0xF1A483D58A02AE65UL ? "OK" : "BAD") << " (" << dhash << ")\n";
}

/**************************************************************************************************/

int main(int argc, const char * argv[]) {
    test0();

    std::cout << "-=-=-=-=-\n";

    test1();

    return 0;
}

/**************************************************************************************************/
