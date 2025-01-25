#include <stlab/test/model.hpp>

using namespace stlab;

auto make_move_only() {
    return move_only(42);
}

int main(int, char**) {
    auto a = make_move_only();
}
/*
    Result:

        move_only ctor
        move_only dtor
*/
