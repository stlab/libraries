---
layout: library
title: scope.hpp
owner: sean-parent
brief: Explicitly scope the lifetime of an object
tags:
  - sourcefile
library-type: sourcefile
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
