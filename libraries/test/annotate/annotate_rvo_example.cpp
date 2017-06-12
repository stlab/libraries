#include <stlab/test/model.hpp>

using namespace stlab;

auto make_annotate() {
    return annotate();
}

int main(int, char**) {
    auto a = make_annotate();
}
/*
    Result:

        annotate ctor
        annotate dtor
*/
