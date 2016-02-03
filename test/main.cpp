/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <iostream>
#include <stlab/future.hpp>

using namespace stlab;
using namespace std;

int main() {
    cout << "start tasking test" << endl;
    auto f = async(default_scheduler(), []{ cout << "Hello from beyond!" << endl; });
    f.detach();
    // while (!f.get_try());
}