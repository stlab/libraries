#include <iostream>
#include <string>

#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>

/**************************************************************************************************/

using namespace stlab;

/**************************************************************************************************/

void test0() {
    std::vector<std::string> output;
    std::mutex               m;
    serial_queue_t           a(stlab::default_executor);
    serial_queue_t           b(stlab::default_executor);
    serial_queue_t           c(stlab::default_executor);
    serial_queue_t           d(stlab::default_executor);
    auto                     aq(a.executor());
    auto                     bq(b.executor());
    auto                     cq(c.executor());
    auto                     dq(d.executor());

    auto strout([&](std::string str) {
        std::lock_guard<std::mutex> l(m);
        output.emplace_back(std::move(str));
    });

    aq([&](){
        strout("a1                      ( 1)");
    });

    bq([&](){
        strout("   b1                   ( 2)");
    });

    dq([&](){
        strout("           d1           ( 3)");
    });

    b([&](){
        strout("   b2                   ( 4)");
    }).then(stlab::immediate_executor, [&](){
        strout("   b2.1                 ( 4.1)");
    }).detach();

    cq([&](){
        strout("        c1              ( 5)");
    });

    cq([&](){
        strout("        c2              ( 6)");
    });

    dq([&](){
        strout("           d2           ( 7)");
    });

    cq([&](){
        strout("        c3              ( 8)");
    });

    bq([&](){
        strout("   b3                   ( 9)");
    });

    aq([&](){
        strout("a2                      (10)");
    });

    aq([&](){
        strout("a3                      (11)");
    });

    dq([&](){
        strout("           d3           (12)");
    });

    while (true) {
        std::lock_guard<std::mutex> l(m);

        if (output.size() == 13)
            break;
    }

    for (const auto& s : output)
        std::cout << s << '\n';
}

/**************************************************************************************************/

inline std::uint64_t str_hash(const std::string& x) {
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

struct serial_hash {
    serial_queue_t   _q{stlab::default_executor};
    std::atomic<int> _c{0};
    std::string      _name;
    std::uint64_t    _h;

    explicit serial_hash(std::string s) : _name(std::move(s)), _h(str_hash(_name)) { }

    void operator()(const std::string& s) {
        _q.executor()([&](){
            _h = hash_combine(_h, s);
            ++_c;
        });
    }

    bool processed(int c) const { return _c >= c; }

    void confirm(std::uint64_t expected) {
        std::cout << _name << " hash " << (_h == expected ? "OK" : "BAD") << " (" << _h << ")\n";
    }
};

/**************************************************************************************************/

void test1() {
    serial_hash a("a");
    serial_hash b("b");
    serial_hash c("c");
    serial_hash d("d");

    a("1");
    b("1");
    d("1");
    b("2");
    c("1");
    c("2");
    d("2");
    c("3");
    b("3");
    a("2");
    a("3");
    d("3");

    while (!a.processed(3)) { }
    a.confirm(0xF1A486D58A02A59AUL);

    while (!b.processed(3)) { }
    b.confirm(0xF1A485D58A02B8B3UL);

    while (!c.processed(3)) { }
    c.confirm(0xF1A484D58A02A6E4UL);

    while (!d.processed(3)) { }
    d.confirm(0xF1A483D58A02AE65UL);
}

/**************************************************************************************************/

int main(int argc, const char * argv[]) {
    test0();

    std::cout << "-=-=-=-=-\n";

    test1();

    return 0;
}

/**************************************************************************************************/
