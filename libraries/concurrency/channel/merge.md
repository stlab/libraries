---
layout: free-function
title: stlab::merge
tags: [library]
pure-name: merge
brief: Creates a future that merges all passed arguments
entities:
  - kind: overloads
    defined-in-header: stlab/concurrency/channel.hpp 
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/channel.hpp 
    list:
      - name: merge
        pure-name: merge
        declaration: |
            template <typename E, typename F, typename...R>
            auto merge(E e, F f, R&&... upstream_receiver)
        description: This function creates a process that executes the provided function object whenever an upstream process provides a value. There is no defined order in which the process `f` is called with the incoming upstream values.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the process. All results from the upstream process must be convertable to the only argument of the provided function object's function operator or the argument of process' await function.
      - name: upstream_receiver
        description: The upstream receiver. 
  - kind: result
    description: a future that merges all passed arguments
---


### Example ###

~~~ c++
#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

int main() {
    sender<int> send1, send2, send3;
    receiver<int> receive1, receive2, receive3;

    tie(send1, receive1) = channel<int>(default_executor);
    tie(send2, receive2) = channel<int>(default_executor);
    tie(send3, receive3) = channel<int>(default_executor);

    std::atomic_int all_done{ 0 };

    auto merged = merge(default_executor, 
                        [](int x) { return x; }, 
                        receive1, 
                        receive2, 
                        receive3)
        | [&_all_done = all_done](int x) { 
              cout << x << '\n';
              ++_all_done; 
          };

    receive1.set_ready();
    receive2.set_ready();
    receive3.set_ready();

    send1(1);
    send3(3);
    send2(2);
    send2(5);
    send3(6);
    send1(4);

    while (all_done < 6) {
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
~~~

### Output ###
The order of the value is not defined!

~~~
1
3
2
4
5
6
~~~
