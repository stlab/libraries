#include <iostream>
#include <string>

#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;

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

void test1() {
    serial_queue_t         aq(stlab::default_executor);
    serial_queue_t         bq(stlab::default_executor);
    serial_queue_t         cq(stlab::default_executor);
    serial_queue_t         dq(stlab::default_executor);
    std::hash<std::string> str_hash;

    std::size_t ahash(str_hash("a"));
    std::size_t bhash(str_hash("b"));
    std::size_t chash(str_hash("c"));
    std::size_t dhash(str_hash("d"));

    auto hash_combine([str_hash](std::size_t hash, const std::string& x){
        return hash ^ (str_hash(x) << 1);
    });

    aq([&](){
        ahash = hash_combine(ahash, "1");
    });

    bq([&](){
        bhash = hash_combine(bhash, "1");
    });

    dq([&](){
        dhash = hash_combine(dhash, "1");
    });

    bq([&](){
        bhash = hash_combine(bhash, "2");
    });

    cq([&](){
        chash = hash_combine(chash, "1");
    });

    cq([&](){
        chash = hash_combine(chash, "2");
    });

    dq([&](){
        dhash = hash_combine(dhash, "2");
    });

    cq([&](){
        chash = hash_combine(chash, "3");
    });

    bq([&](){
        bhash = hash_combine(bhash, "3");
    });

    aq([&](){
        ahash = hash_combine(ahash, "2");
    });

    aq([&](){
        ahash = hash_combine(ahash, "3");
    });

    dq([&](){
        dhash = hash_combine(dhash, "3");
    });

    std::cout << "a: " << ahash << (ahash == std::size_t(14189587631650534178) ? " (OK)" : " (BAD) ") << '\n';
    std::cout << "b: " << bhash << (bhash == std::size_t(11845100715899112661) ? " (OK)" : " (BAD) ") << '\n';
    std::cout << "c: " << chash << (chash == std::size_t(8461684262208515064) ? " (OK)" : " (BAD) ") << '\n';
    std::cout << "d: " << dhash << (dhash == std::size_t(17611461363091444353) ? " (OK)" : " (BAD) ") << '\n';
}

int main(int argc, const char * argv[]) {
    test0();

    test1();

    return 0;
}
