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
    auto f1 = async(default_scheduler(), []{ cout << "Hello from beyond!" << endl; });
    f1.detach();

    cout << "start tasking test" << endl;
    auto myAnswer = make_ready_future([]() { return 42; });
    auto myBigAnswer = make_ready_future([]() { return 84; });
    auto myCombination = [](int a) ->int { return a; };
    auto myResult = [](int answer) { std::cout << "The answer is" << answer << std::endl; };
    auto f2 = when_all(default_scheduler(), myCombination, myAnswer, myBigAnswer).then(myResult);
    f2.detach();
    // while (!f.get_try());
}