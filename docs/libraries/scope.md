---
layout: function
title: stlab::scope
tags: [library]
scope: stlab
pure-name: scope
brief: Explicitly scope the lifetime of an object
annotation: template function
example: [ scope_example.cpp, scope_example_return.cpp ]
defined-in-header: stlab/scope.hpp
entities:
  - kind: overloads
    list:
      - name: scope
        pure-name: scope
        declaration: |
            template <typename T, typename... Args>
            inline auto scope(Args&&... args)
        description: Scopes the lifetime of an instance of `T`. All but the last parameters are used to construct `T`, while the last parameter is assumed to be a nullary function, and is called. After the nullary function goes out of scope, `T` is destroyed.
  - kind: parameters
    list:
      - name: |
            [ args[0], args[N-1] )
        description: Range of parameters forwarded to the constructor of `T`
      - name: args[N-1]
        description: A callable nullary function
---

Oftentimes developers will add scopes to code where they want to limit the lifetime of an object. For example:

~~~c++
void pop_and_run_task() {
    std::function<void()> task;

    {
        std::lock_guard<std::mutex> lock(m);
        task = pop_front_unsafe(task_queue);
    }

    task();
}
~~~

The challenge is discerning the developer's intent by adding the scope. `scope` allows construction of any object and binds its lifetime to the duration of a passed function. Using `scope`, the example above becomes:

~~~c++
void pop_and_run_task() {
    std::function<void()> task = stlab::scope<std::lock_guard<std::mutex>>(m, [&](){
        return pop_front_unsafe(task_queue);
    });

    task();
}
~~~

With `scope`, the developer's intent is clear: they want the lifetime of the lock to be bound to the scope that encloses it.
