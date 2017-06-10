---
layout: free-function
title: stlab::scope
tags: [library]
scope: stlab
pure-name: scope
brief: Explicitly scope the lifetime of an object
annotation: template function
example: scope_example.cpp
entities:
  - kind: overloads
    defined-in-header: stlab/concurrency/scope.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/scope.hpp
    list:
      - name: scope
        pure-name: scope
        declaration: |
            template <typename T, typename... Args>
            inline void scope(Args&&... args)
        description: Scopes the lifetime of an instance of `T` with parameters `args[0]...args[N-2]`, then executes `args[N-1]` which is assumed to be a callable nullary function.
  - kind: parameters
    list:
      - name: args[0] ... args[N-2]
        description: Parameters forwarded to the instance of `T` constructor
      - name: args[N-1]
        description: A callable nullary function
---

Oftentimes developers will add scopes to code where they want to limit the lifetime of an object. For example:

~~~c++
// Assuming mutex m and task_queue...
void async_run() {
    std::function<void()> task;

    {
    std::lock_guard<std::mutex> lock(m); // acquire the mutex
    task = pop_front_unsafe(task_queue);
    } // unlock the mutex

    task();
}
~~~

While the code may be correct, it is hard to discern the developer's intent to link the free-standing scope with the `lock_guard` instance. `stlab::scope` allows construction of any object and bind its lifetime to the duration of a passed function. The example above becomes:

~~~c++
// Assuming mutex m and task_queue...
void async_run() {
    std::function<void()> task;

    stlab::scope<std::lock_guard<std::mutex>>(m, [&](){
        task = pop_front_unsafe(task_queue);
    });

    task();
}
~~~

The developer's intent is clear: they want the lifetime of the lock to be bound to the enclosing scope.
