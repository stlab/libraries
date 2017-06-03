---
layout: free-function
title: stlab::join
tags: [library]
pure-name: join
brief: Creates a future that joins all passed arguments
entities:
  - kind: overloads
    defined-in-header: stlab/channel.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
    list:
      - name: join
        pure-name: join
        defined-in-header: stlab/channel.hpp
        declaration: |
            template <typename E, typename F, typename...R>
            auto join(E e, F f, R&&... upstream_receiver)
        description: This function creates a new receiver and attaches the process `f` to it. The values coming from the upstream receiver are the parameters of `f`. The incoming upstream values are not passed one after the other to this process, but they are passed as a complete set of arguments to the process. So the last incoming upstream value triggers the execution of `f`.
  - kind: parameters
    list:
      - name: e
        description: Executor which shall be used to execute the task of `f`.
      - name: f
        description: Callable object that implements the task. Its parameters correspond to the results from the upstream receivers. It is called when all upstream receiver have provided its values.
      - name: upstream_receiver
        description: The upstream receiver.
  - kind: result
    description: a future that joins all passed arguments
---

### Example ###

~~~ c++
#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/channel.hpp>
#include <stlab/default_executor.hpp>

using namespace std;
using namespace stlab;

int main() {
    sender<int> send1, send2;
    receiver<int> receive1, receive2;

    tie(send1, receive1) = channel<int>(default_executor);
    tie(send2, receive2) = channel<int>(default_executor);

    std::atomic_bool done{ false };

    auto joined = join(default_executor, 
                      [](int x, int y) { return x + y; }, // x will get 1, y will get 2
                      receive1, 
                      receive2)
        | [&_done = done](int x) { 
              cout << x << '\n'; 
              _done = true; 
          };

    receive1.set_ready();
    receive2.set_ready();

    send1(1);
    send2(2);

    while (!done) {
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
~~~

### Output ###
~~~
3
~~~
