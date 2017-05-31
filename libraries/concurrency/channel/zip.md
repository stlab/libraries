---
layout: free-function
title: stlab::zip
tags: [library]
entities:
  - kind: overloads
    defined-in-header: stlab/channel.hpp  
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
    list:
      - name: zip
        pure-name: zip
        declaration: |
            template <typename E, typename F, typename...R>
            auto zip(E e, F f, R&&... upstream_receiver)
        description: This function creates a process that executes the provided function object with the results from the upstream process. The results are passed in a round-robin manner, starting with the result from the first receiver.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task. All results from the upstream process must be convertable to the only argument of the provided function object.
      - name: upstream_receiver
        description: The upstream receiver(s). 
  - kind: result
    description: a future that zips all passed arguments
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
    sender<int> send1, send2, send3;
    receiver<int> receive1, receive2, receive3;

    tie(send1, receive1) = channel<int>(default_executor);
    tie(send2, receive2) = channel<int>(default_executor);
    tie(send3, receive3) = channel<int>(default_executor);

    std::atomic_int all_done{ 0 };

    auto zipped = zip(default_executor, 
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

    // even the values are send in mixed order into the channels, the are passed
    // in round robin manner into the zipped process
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
{: .example_box}

### Output ###
~~~
1
2
3
4
5
6
~~~
{: .example_box}
