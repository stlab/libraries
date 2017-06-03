---
layout: class
title: stlab::buffer_size
tags: [library]
pure-name: buffer_size
dtor: default
name: stlab::buffer_size
defined-in-header: stlab/channel.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
declaration: struct buffer_size
brief: Encapsulates the buffer size of a process
description: This class encapsulates the buffer size of a process.
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

    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
~~~
