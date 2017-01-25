---
layout: free-function
title: stlab::when_all
tags: [library]
entities:
  - kind: overloads
    defined-in-header: stlab/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/future.hpp
    list:
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename...Ts>
            auto when_all(E e, F f, future<Ts>... args)
        description: This function create a joining future. When all passed `args` futures are fullfilled, then the continuation tasks defined with `f` is scheduled on the executor `e`.
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename I> 
            auto when_all(E e, F f, const std::pair<I,I>& range)
        description: This function create a joining future 
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task
      - name: args
        description: Futures that shall be joined
      - name: range
        description: a range of futures of type `U`, specified by a half open range. All futures must succeed, before the continuation is triggered. It takes a `vector<U>` filled with all results as result.
  - kind: result
    description: a future that joins all passed arguments and passes them to the associated function object
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
    auto argument1 = async(default_executor, [] { return 42; });
    auto argument2 = async(default_executor, [] { return string("The answer is"); });

    auto result = when_all(default_executor, [](int answer, std::string text) {
        cout << text << " " << answer << '\n'
    }, argument1, argument2);

    // Waiting just for illustrational purpose
    while (!result.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
}
~~~
{: .example_box}

### Result ###

~~~
The answer is 42
~~~
{: .example_box}
