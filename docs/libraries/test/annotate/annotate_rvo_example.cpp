#include <stlab/test/model.hpp>

using namespace stlab;

annotate_counters counters;

auto make_annotate() {
    return annotate(counters);
}

int main(int, char**) {
    {
        auto a = make_annotate();
    }
    std::cout << counters;
}
/*
    Result:
		           dtor: 1
		      copy_ctor: 0
		      move_ctor: 0
		copy_assign_lhs: 0
		copy_assign_rhs: 0
		move_assign_lhs: 0
		move_assign_rhs: 0
		           swap: 0
		       equality: 0
*/
