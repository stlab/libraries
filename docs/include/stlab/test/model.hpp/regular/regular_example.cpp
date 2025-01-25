#include <stlab/test/model.hpp>

using namespace stlab;

auto make_regular(int x) {
    return regular(x);
}

int main(int, char**) {
    auto a = make_regular(21);
    auto b = a;

    a = make_regular(42);

    (void)(a < b);
}
/*
    Result:

        21 ctor
        21 copy-ctor
        42 ctor
        21 move-assign
        0 dtor
        42 < 21: false
        21 dtor
        42 dtor
*/
