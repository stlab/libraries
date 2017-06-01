---
layout: class
title: stlab::executor
tags: [library]
entities:
  - kind: class
    name: stlab::executor
    pure-name: executor
    defined-in-header: stlab/channel.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
    declaration: struct executor
    description: This class encapsulates an executor to be piped to a `receiver<T>`
    member-functions:
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

    // The process times_two will be executed immediately
    // The order of the process and executor is not relevant so calling
    // times_two & executor{ immediate_executor } would be equivalent
    auto result = receive 
        | executor{ immediate_executor } & times_two
        | [&v](int x) { v = x; };
        
    receive.set_ready();

    send(1);

    // Waiting just for illustrational purpose
    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
~~~
{: .example_box}
