---
layout: free-function
title: stlab::channel
tags: [library]
entities:
  - kind: overloads
    defined-in-header: stlab/channel.hpp
    list:
      - name: channel
        pure-name: channel
        declaration: |
            template <typename T, typename E>
            std::pair<sender<T>, receiver<T>> channel(E e)
        description: This function creates a channel that consists of a sending and a receiving part of the channel.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to sends the passed values from the sender down to the receiver.
  - kind: result
    description: Returns a pair of `sender` - `receiver` of type `T` that form a channel
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

    // It is necessary to mark the receiver side as ready, when all connections are
    // established
    receive.set_ready();

    send(1);
    send(2);
    send(3);

    // Waiting just for illustrational purpose
    while (!done.load()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
~~~

### Output ###

~~~
1
2
3
~~~
