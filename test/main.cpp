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

    cout << "Let's have a chain of tasks" << endl;
    auto theAnswer = make_ready_future([]() { return 42; });
    auto twiceTheAnswer = make_ready_future([]() { return 42 * 42; });
    auto combinationOfTwoAnswers = [](int a, int b) { return a + b; };
    auto myResult = [](int answer) { std::cout << "The answer is" << answer << std::endl; };
    auto f2 = when_all(default_scheduler(), combinationOfTwoAnswers, theAnswer, twiceTheAnswer).then(myResult);
    f2.detach();
    // while (!f.get_try());
}