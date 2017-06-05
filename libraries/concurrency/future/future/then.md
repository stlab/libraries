---
layout: method
title: then
tags: [library]
pure-name: then
defined-in-header: stlab/concurrency/future.hpp
brief: Creates a continuation
declaration: then()
description: |
  Creates a new continuation on the current object. 

  Multiple `then()` invocations on the same future object realize a split. The result of this will be copied into all continuations. 

  If `T` of `future<T>` is a move only type, then a split is not possible of course.
entities:
  - kind: methods
    list:
      - name: stlab::future::get_try
        pure-name: get_try
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F> 
          auto then(F&& f) const&
        description: Constructs a continuation with the passed callable object using the same executor as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto then(E&& e, F&& f) const&
        description: Constructs a continuation with the passed callable object using the provided executor.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F>
          auto then(F&& f) &&
        description: Constructs a continuation on a r-value with the passed callable object using the same executor as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto then(E&& e, F&& f) &&
        description: Constructs a continuation on a r-value with the passed callable object using the provided executor.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
  - kind: result
    description: a future of the type of the result type of the passed function object.
  - kind: example
    code: NoCode
---

### Future Continuation Example ###

~~~ c++
#include <cstdio>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto x = async(default_executor, [] { return 42; });

    auto y = x.then([](int x) { printf("Result %d \n", x); });

    // Waiting just for illustrational purpose
    while (!y.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
}
~~~

### Result ###

~~~
Result 42
~~~

### Future Split Example ###

~~~ c++
#include <cstdio>
#include <thread>
#include <stlab/default_executor.hpp>
#include <stlab/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto x = async(default_executor, [] { return 42; });

    auto c1 = x.then([](int x) { printf("Split A %d \n", x); });
    auto c2 = x.then([](int x) { printf("Split B %d \n", x); });

    // Waiting just for illustrational purpose
    while (!c1.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
    while (!c2.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
}
~~~

### Result ###

The order of the results is not defined.

~~~
Split B 42
Split A 42
~~~
