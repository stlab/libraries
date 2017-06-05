---
layout: free-function
title: stlab::async
tags: [library]
pure-name: async
brief: Run a function on a specified executor
annotation: template function
entities:
  - kind: overloads
    name: stlab::async
    defined-in-header: stlab/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/future.hpp
    list:
      - name: async
        pure-name: async
        declaration: |
            template <typename E, typename F, typename ...Args>
            future<std::result_of_t<F (Args...)>> async(E executor, F&& f, Args&&... args)
        description: The template function async runs the function f asynchronously on the specified executor and returns a stlab::future that will eventually hold the result of that function call.
  - kind: parameters
    list:
      - name: executor
        description: The passed function will run on this executor
      - name: f
        description: Callable object to call
      - name: args
        description: parameters to pass to f
  - kind: result
    description: stlab::future referring to the shared state created by this call to std::async.
---

### Example ###

~~~ c++
#include <cstdio>
#include <string>
#include <thread>
#include <stlab/default_executor.hpp>
#include <stlab/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto f = async(default_executor, [] { return 42; });
 
    // Waiting just for illustrational purpose
    while (!f.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    cout << "The answer is " << f.get_try().value() << "\n";
}
~~~

### Result ###

~~~
The answer is 42
~~~
