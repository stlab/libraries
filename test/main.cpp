/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <iostream>
#include <stlab/future.hpp>
#include <utility>

using namespace stlab;
using namespace std;

int main() {
    cout << "start tasking test" << endl;
    auto f1 = async(default_scheduler(), [] { return 42; }).then( 
      [](auto answer) { cout << "The Answer to the Ultimate Question of Life, the Universe and Everything is " << answer << endl << flush; });
    f1.detach();
    

    auto f2 = async(default_scheduler(), [] { return 10; });
    auto a1 = f2.then([](auto x) { return x + 2; });
    auto a2 = f2.then([](auto x) { return x + 3; });
    auto a4 = when_all(default_scheduler(), [](auto x, auto y) {
      cout << x << ", " << y << endl;
    }, a1, a2);
    a4.detach();

    std::vector<stlab::future<int>> emptyFutures;
    auto a5 = when_all(default_scheduler(), [](std::vector<int> v) {
      cout << "Result of no parallel tasks: " << v.size() << endl << flush;
    }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));
    a5.detach();

    std::vector<stlab::future<int>> someFutures;
    someFutures.push_back(async(default_scheduler(), [] { return 1; }));
    someFutures.push_back(async(default_scheduler(), [] { return 2; }));
    someFutures.push_back(async(default_scheduler(), [] { return 3; }));
    someFutures.push_back(async(default_scheduler(), [] { return 4; }));

    auto a6 = when_all(default_scheduler(), [](std::vector<int> v) {
      cout << "Result of "<< v.size() <<" parallel executed tasks: ";
      for (auto i : v) {
        cout << i << " ";
      } 
      cout << endl << flush;
    }, std::make_pair(someFutures.begin(), someFutures.end()));
    a6.detach();
}