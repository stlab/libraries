---
layout: method
title: process::await
tags: [library]
full-name: process::await
pure-name: await
declaration: void await(T... val)
description: This method is called on an await-process whenever a new value was received from upstream. The type `T` stands here for any semi regular or move-only type. The number of arguments depends on  the number of attached upstream sender. Potential state changes from awaitable to yieldable should happen while this method is invoked.
entities:
  - kind: methods
    list:
      - name: process::await
        pure-name: await
        declaration: void await(T... val)
        description: This method is called with the next set of values
  - kind: parameters
    list:
      - name: val
        description: The value(s) that are passed to `await`
  - kind: result
---

### Example ###

~~~ c++
#include <atomic>
#include <thread>
#include <stlab/channel.hpp>
#include <stlab/default_executor.hpp>

struct sum_2 
{
    process_state_scheduled _state = await_forever;
    int _counter = 0;
    int _sum = 0;

    void await(int n) { 
        _sum += n; 
        ++_counter;
        if (_counter == 2) _state = yield_immediate;
    }

    int yield() { 
      _state = await_forever;
      auto result = _sum;
      _sum = 0;
      _counter = 0;
      return _sum; 
    }

    auto state() const { return _state; }
};


int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int r = 0;

    auto hold = receive
        | sum_2()
        | [&_r = r](int x) { _r = x; };

    receive.set_ready();

    send(1);
    send(2);

    // Waiting just for illustrational purpose
    while (r == 0) {
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    cout << "The sum is " << r.load() << '\n';
}
~~~
{: .example_box}

### Result ###

~~~
The sum is 3
~~~
{: .example_box}
