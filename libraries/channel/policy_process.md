---
layout: class
title: process
tags: [library]
entities:
  - kind: class
    name: process
    tag: no constructor and destructor
    pure-name: process
    description: |
      The process is a policy that must follow certain rules and no a concrete type.

      Each channel is associated with a process. The process can either be


        - a n-ary function object, further here called a function-process. 
        - a type that must has properties, that are described below. It is further here called an await-process.
      Since a function-process does not need further explication, the following concentrates on await-processes.
    member-functions:
      - name: await
        description: Awaits the new value for this process
        pure-name: await
      - name: close
        description: Closes the processing of this process
        pure-name: close
      - name: state
        description: Returns the current state of the process
        pure-name: state
      - name: yield
        description: Yields the process result
        pure-name: yield
---

### Example ###

~~~ c++
struct sum {
    process_state_scheduled _state = await_forever;
    int _sum = 0;

    void await(int n) { _sum += n; }

    int yield() { _state = await_forever; return _sum; }

    void close() { _state = yield_immediate; }

    const auto& state() const { return _state; }
};

~~~
{: .example_box}
