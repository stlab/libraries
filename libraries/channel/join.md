---
layout: free-function
title: stlab::join
tags: [library]
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
        description: This function creates a process that executes the provided process and passes the results from the upstream receiver to this one.
  - kind: parameters
    list:
      - name: e
        description: Executor which shall be used to execute the resulting task
      - name: f
        description: Callable object that implements the task. Its parameters correspond to the results from the upstream receivers. It is called when all upstream receiver have provided its values.
      - name: upstream_receiver
        description: The upstream receiver.
  - kind: result
    description: a future that joins all passed arguments
---

## Example ##

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