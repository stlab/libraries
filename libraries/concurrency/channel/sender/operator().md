---
layout: method
title: stlab::sender
tags: [library]
full-name: stlab::sender::operator()
pure-name: operator()
defined-in-header: stlab/channel.hpp 
declaration: operator()
description: Sends a new value into the channel
entities:
  - kind: methods
    list:
      - name: stlab::sender::operator()
        pure-name: operator()
        defined-in-header: stlab/channel.hpp 
        declaration: |
          template <typename... A> 
          void operator()(A&&... args) const
        description: Sends a new value into the channel.
  - kind: parameters
    list:
      - name: args
        description: The input values of the channel that is defined by this sender
  - kind: result
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
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_bool done{ false };

    auto hold = receive | [&_done = done](int x) {  
        cout << x << '\n'; 
        _done = true; 
      };

    // It is necessary to mark this side ready, when all connections are
    // established
    receive.set_ready();

    // calling operator() on send
    send(1);
    send(2);
    send(3);

    // Waiting just for illustrational purpose
    while (!done.load()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
~~~
{: .example_box}

### Output ###

~~~
1
2
3
~~~
{: .example_box}