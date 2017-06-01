---
layout: method
title: stlab::buffer_size
tags: [library]
full-name: stlab::buffer_size::buffer_size
pure-name: buffer_size
defined-in-header: stlab/channel.hpp 
declaration: buffer_size()
description: Constructs a new buffer_size object
entities:
  - kind: methods
    list:
      - name: stlab::buffer_size::buffer_size
        pure-name: buffer_size
        defined-in-header: stlab/channel.hpp 
        declaration: buffer_size(std::size_t sz)
        description: Constructs a new buffer_size object and initializes with the value sz.
  - kind: parameters
    list:
      - name: sz
        description: Specifies the size of the buffer
  - kind: example
    code: NoCode
---

### Example ###

~~~ c++
#include <atomic>
#include <thread>
#include <stlab/channel.hpp>
#include <stlab/immediate_executor.hpp>
#include <stlab/default_executor.hpp>

int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int v{0};

    auto times_two = [](int x) { return x * 2; };

    // The order of the process and buffer_size is not relevant, so calling
    // times_two & buffer_size{ 2 } would be equivalent
    auto result = receive 
        | buffer_size{ 2 } & times_two
        | [&v](int x) { v = x; };
        
    receive.set_ready();

    send(1);

    // Waiting just for illustrational purpose
    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
~~~
