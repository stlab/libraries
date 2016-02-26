/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <iostream>
#include <stlab/future.hpp>
#include <utility>
//#include <gtest/gtest.h>

using namespace stlab;
using namespace std;

void simple_continuation() {
    cout << "start tasking test" << endl;
    auto f1 = async(default_scheduler(), [] { return 42; }).then(
        [](auto answer) { cout << "The Answer to the Ultimate Question of Life, the Universe and Everything is " << answer << endl << flush; });
    f1.detach();
}

void when_all_with_multiple_arguments() {
    auto f2 = async(default_scheduler(), [] { return 10; });
    auto a1 = f2.then([](auto x) { return x + 2; });
    auto a2 = f2.then([](auto x) { return x + 3; });
    auto a4 = when_all(default_scheduler(), [](auto x, auto y) {
        cout << x << ", " << y << endl;
        return 4711;
    }, a1, a2);
    //a4.detach();
    while (!a4.get_try()) {}
    auto result = a4.get_try();
    cout << "Result " << *result << endl;
}

void when_all_with_empty_range() {
    std::vector<stlab::future<int>> emptyFutures;
    auto a5 = when_all(default_scheduler(), [](std::vector<int> v) {
        cout << "Result of no parallel tasks: " << v.size() << endl << flush;
    }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));
    a5.detach();
}

void when_all_with_filled_range() {
    std::vector<stlab::future<int>> someFutures;
    someFutures.push_back(async(default_scheduler(), [] { return 1; }));
    someFutures.push_back(async(default_scheduler(), [] { return 2; }));
    someFutures.push_back(async(default_scheduler(), [] { return 3; }));
    someFutures.push_back(async(default_scheduler(), [] { return 4; }));

    auto a6 = when_all(default_scheduler(), [](std::vector<int> v) {
        cout << "Result of " << v.size() << " parallel executed tasks: ";
        for (auto i : v) {
            cout << i << " ";
        }
        cout << endl << flush;
        return 4711;
    }, std::make_pair(someFutures.begin(), someFutures.end()));

    while (!a6.get_try()) {}
    auto result = a6.get_try();
    cout << "Result " << *result << endl;
}

void continuation_with_error() {
    cout << "start tasking test" << endl;
    auto f1 = async(default_scheduler(), []() -> int { throw std::exception("Error in continuation"); }).then(
        [](auto answer) { cout << "The Answer to the Ultimate Question of Life, the Universe and Everything is " << answer << endl << flush; });

    try {
        while (!f1.get_try()) {}
    }
    catch (const std::exception& e) {
        cout << __FUNCTION__ << " " << e.what() << endl;
    }
}

void when_all_with_multiple_failing_arguments() {
    auto f2 = async(default_scheduler(), [] { return 10; });
    auto a1 = f2.then([](auto x) -> int { throw std::exception("Error in first argument"); });
    auto a2 = f2.then([](auto x) { return x + 3; });
    auto a4 = when_all(default_scheduler(), [](auto x, auto y) {
        cout << x << ", " << y << endl;
    }, a1, a2);

    try {
        while (!a4.get_try()) {}
    }
    catch (const std::exception& e) {
        cout << __FUNCTION__ << " " << e.what() << endl;
    }
}

void when_all_with_failing_range() {
    std::vector<stlab::future<int>> someFutures;
    someFutures.push_back(async(default_scheduler(), [] { return 1; }));
    someFutures.push_back(async(default_scheduler(), []() ->int { throw std::exception("Error in 2nd task"); }));
    someFutures.push_back(async(default_scheduler(), [] { return 3; }));
    someFutures.push_back(async(default_scheduler(), [] { return 4; }));

    auto a6 = when_all(default_scheduler(), [](std::vector<int> v) {
        cout << "Result of " << v.size() << " parallel executed tasks: ";
        for (auto i : v) {
            cout << i << " ";
        }
        cout << endl << flush;
    },
        std::make_pair(someFutures.begin(), someFutures.end()));

    try {
        while (!a6.get_try()) {}
    }
    catch (const std::exception& e) {
        cout << __FUNCTION__ << " " << e.what() << endl;
    }
}


int main(int argc, char **argv)
{
    //std::cout << "Running main() from gtest_main.cc\n";
    //::testing::InitGoogleTest(&argc, argv);

    simple_continuation();
    when_all_with_multiple_arguments();
    when_all_with_empty_range();
    when_all_with_filled_range();
    continuation_with_error();
    when_all_with_multiple_failing_arguments();
    when_all_with_failing_range();

    //return RUN_ALL_TESTS();
}
