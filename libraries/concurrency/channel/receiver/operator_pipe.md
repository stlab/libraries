---
layout: method
title: stlab::receiver
tags: [library]
full-name: stlab::receiver::operator|
pure-name: operator|
defined-in-header: stlab/channel.hpp 
declaration: operator|()
description: Creates a new receiver, attaches the given process as downstream to it and returns this new receiver. The new receiver inherits the executor from its upstream receiver if not an alternative executor is attached.
entities:
  - kind: methods
    list:
      - name: stlab::receiver::operator|
        pure-name: operator|
        defined-in-header: stlab/channel.hpp 
        declaration: |
          template <typename F> 
          auto operator|(F&& f)
        description: Attaches the given process to the receiver and returns a new receiver. It can either be a function object with a single parameter or a process that has an `await()` and a `yield()` method.
      - name: stlab::receiver::operator|
        pure-name: operator|
        defined-in-header: stlab/channel.hpp 
        declaration: |
          template <typename F>
          auto operator|(detail::annotated_process<F>&& ap)
        description: Attaches the given annotated process to the receiver and returns a new receiver. It can either be a function object with a single parameter or a process that follows this signatur. An `annotated_process` should not be explicitly instantiated but it gets implicitly created as soon as one combines a function object with an `operator&` and an `executor` or a `buffer_size` or both.
  - kind: parameters
    list:
      - name: f
        description: The process to be executed.
      - name: args
        description: Futures that shall be joined
  - kind: result
    description: A receiver of type of the result of the passed function object
  - kind: utilitys
    classes:
      - name: executor
        defined-in-header: stlab/channel.hpp
        description: Encapsultes an executor to be used by a process
        pure-name: executor
        link: ../executor/index
      - name: buffer_size
        defined-in-header: stlab/channel.hpp
        description: Encapsulates the buffer size of a process
        pure-name: buffer_size/index
        link: buffer_size
---


### Example ###

~~~ c++
#include <atomic>
#include <thread>
#include <stlab/channel.hpp>
#include <stlab/default_executor.hpp>
#include <stlab/main_executor.hpp>

int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int v{0};

    // The code demonstrates how a process can be annotated
    auto result = receive 
        | buffer_size{ 3 } & [](int x) { return x * 2; }
        | [](int x) { return x * 2; } & buffer_size{ 2 }
        | buffer_size{ 3 } & executor{ default_executor } & [](int x) { return x * 2; } 

        | executor{ default_executor } & [](int x) { return x + 1; }
        | [](int x) { return x + 1; } & executor{ main_executor }
        | executor{ default_executor } & buffer_size{ 3 } & [](int x) { return x * 2; }
    
        | [](int x) { return x + 1; } & executor{ default_executor } & buffer_size{ 3 }
        | [](int x) { return x * 2; } & buffer_size{ 3 } & executor{ main_executor }
        
        | [&v](int x) { v = x; };
        
    receive.set_ready();

    send(1);

    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
~~~
