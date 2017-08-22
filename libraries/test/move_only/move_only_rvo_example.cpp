#include <stlab/test/model.hpp>

using namespace stlab;

auto make_move_only() {
    return move_only();
}

int main(int, char**) {
    auto a = make_move_only();
}
/*
    Result:

        move_only ctor
        move_only dtor
*/
