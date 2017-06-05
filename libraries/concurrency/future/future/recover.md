---
layout: method
title: recover
tags: [library]
pure-name: recover
defined-in-header: stlab/concurrency/future.hpp 
declaration: recover()
description: Creates a recoverable continuation on the current object.
entities:
  - kind: methods
    list:
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F> 
          auto recover(F&& f) const&
        description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the same executor as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto recover(E&& e, F&& f) const&
        description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F>
          auto recover(F&& f) &&
        description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the same executor as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto recover(E&& e, F&& f) &&
        description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the the provided executor.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
---

~~~ c++
#include <cstdio>
#include <exception>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto x = async(default_executor, [] { 
      throw exception("Vogons did arrive!"); 
      return 42; 
    });

    auto r = x.recover([](future<int> f) { 
        try {
          auto answer = f.get_try().value();
          cout << "The answer is " answer << '\n';
        }
        catch (const exception& ex) {
          cout << "The error \"" << ex.what() << "\" happened!\n";
        }
      });

    // Waiting just for illustrational purpose
    while (!r.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }
}
~~~

### Result ###

~~~
The error "Vogons did arrive!" happened!
~~~
