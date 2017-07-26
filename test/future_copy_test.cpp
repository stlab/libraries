#include <iostream>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>

using namespace stlab;
using namespace std;

struct annotate
{
    enum operations
    {
        ctor,
        copy_ctor,
        move_ctor,
        assign,
        move_assign,
        swap_,
        dtor,
        last_entry
    };

    annotate() {
        ++ops[ctor];
        cout << "annotate ctor" << endl;
    }

    annotate(const annotate &) {
        ++ops[copy_ctor];
        cout << "annotate copy-ctor" << endl;
    }

    annotate(annotate &&) noexcept {
        ++ops[move_ctor];
        cout << "annotate move-ctor" << endl;
    }

    annotate &operator=(const annotate &) {
        ++ops[assign];
        cout << "annotate assign" << endl;
        return *this;
    }

    annotate &operator=(annotate &&) noexcept {
        ++ops[move_assign];
        cout << "annotate move-assign" << endl;
        return *this;
    }

    ~annotate() {
        ++ops[dtor];
        cout << "annotate dtor" << endl;
    }

    friend inline void swap(annotate &, annotate &) {
        ++ops[swap_];
        cout << "annotate swap" << endl;
    }

    friend inline bool operator==(const annotate &, const annotate &) { return true; }

    friend inline bool operator!=(const annotate &, const annotate &) { return false; }

    static std::atomic_int ops[operations::last_entry];

    static void reset_ops_counter() { for (auto &op : annotate::ops) { op = 0; }}
};

std::atomic_int annotate::ops[annotate::operations::last_entry];

template<class T>
inline auto promise_future() {
    return package<T(T)>(immediate_executor, [](auto &&x) -> decltype(x) { return std::forward<decltype(x)>(x); });
}


int main() {
    const int expected_ops[annotate::last_entry] = {1, 0, 4, 0, 0, 0, 5};
    annotate::reset_ops_counter();
    {
        auto pack = promise_future<annotate>();
        pack.first(annotate());
        pack.second.then([](const annotate &) {});
    }
    for (auto i = 0; i < annotate::last_entry; ++i) {
        if (expected_ops[i] != annotate::ops[i])
            return -1;
    }
}
