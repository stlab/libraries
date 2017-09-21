#include <stlab/test/model.hpp>

using namespace stlab;

annotate_counters counters;

auto make_annotate() {
    return annotate(counters);
}

int main(int, char**) {
    auto a = make_annotate();
}
/*
    Result:

        annotate ctor
        annotate dtor
*/
