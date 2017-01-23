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
      The process is a policy that must follow certain rules; it is not a type that the library provides.

      Each channel is associated with a process. The process can either be


        - an n-ary function object, further here called a function-process. 
        - a type that must have properties, that are described below. It is further here called an await-process.
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

### Signature of an await-process ###

Here it is assumed that the process takes `T...`as argument and its result type is `U`.

~~~ c++
struct process 
{
    void await(T... val);

    U yield();

    void close();

    process_state_scheduled state() const;
};

~~~
{: .example_box}
