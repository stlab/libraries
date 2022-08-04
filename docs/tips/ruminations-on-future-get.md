---
title: Ruminations on `future::get`
layout: page
tags: [tips]
comments: true
draft: true
---

The library `<future>` was released as part of C++11. In some versions of my talk, [Better Code: Concurrency](https://www.youtube.com/watch?v=QIHy8pXbneI), I've stated incorrectly that there was a change between the C++11 and C++14 standards to allow `std::async()` to issue tasks on a thread pool. I've been unable to recall exactly what lead to this change in my talk but thanks to Nicolai Josuttis the error was caught.

Nicolai brought up the issue on the standard library reflector and that led to some interesting conversation and, for me, the realization that there was a more to `std::future`.

Reading the standard for `<future>` there are three ways to make a future with a shared state, via `promise::get_future()`, `packaged_task::get_future()`, or as the result of `async()`. At first glance, it appears that later operations can be implemented in terms of the former. However, that is not the case. A `future` returned by `async()` has properties that cannot be implemented by `promise<>` or `packaged_task<>`.

Specifically:
- On destruction, a `future` returned by `async(launch::async, f, ...)` will block (synchronize) with the thread join if it hasn't already occurred. This means that ignoring the result of `async(launch::asysc, ...)` will behave as an expensive synchronous call.
- A `future` returned by `async(launch::deferred, f, ...)` will invoke `f` when `get()` or `wait()` is called, otherwise `f` is not invoked, even when the `future` is destructed.

If the launch policy is `launch::any`, then the Visual C++ implementation of the standard library will issue an asynchronous call to a thread pool. To avoid a potential deadlock from having the task scheduled within the same pool as the code calling `get()` or `wait()`, the task is _promoted_ to execute immediately (as if deferred) if it has not started execution when one of these operations is called.

I believe this behavior of executing the task within a thread pool is what lead to the initial confusion, but this implementation satisfies the existing standard wording. It also leads to some interesting results.

For example, above it was observed that a `future` returned by `async(launch::deferred, f, ...)` will not invoke `f` if `get()` or `wait()` is not called, this includes if the `future` is destructed. This property is known as _cancelation_. The task is canceled if the value is not read from the future. Cancelation is an important property when writing asynchronous code. It applies the principals of RAII to the processor, which is arguably the most valuable resource in the machine, by ensuring that tasks which are no longer needed are not executed.

Although there is no way (that I'm aware of) to construct a cancelable task from a promise, we can use `async()` and a level of indirection to create one.

{::comment} code should be moved to a compiled file and included. {:/}
```cpp
template <class Sig, class F>
auto cancelable_task(F&& f) {
    using shared_t = std::packaged_task<Sig>;

    auto p = std::make_shared<shared_t>(std::forward<F>(f));
    auto w = std::weak_ptr<shared_t>(p);
    auto r = p->get_future();

    return std::make_pair([_w = std::move(w)] (auto&&... args) {
        if (auto p = _w.lock()) (*p)(std::forward<decltype(args)>(args)...);
    },
    std::async(std::launch::deferred, [_p = std::move(p), _r = std::move(r)] () mutable {
        return _r.get();
    }));
}
```

The `cancelable_task()` function can be used similar to `std::packaged_task<>` to create a task which is cancelable, it becomes a no-op if the future is destructed without calling `get()` or `wait()`. Instead of creating a `packaged_task` instance and then calling `get_future()`, `cancelable_task()` returns a pair of the task and `std::future`. This is done because without the paired future, the task is destructed. (The interface will be a bit simpler to use with C++17 structured bindings.)

Here is an example use that doesn't cancel:

```cpp
auto task_future = cancelable_task<int(int)>([](int x){
	cout << "called: " << x << endl;
	return x;
});
task_future.first(42); // invoke the task
cout << "result: " << task_future.second.get() << endl; // display result
```

This code will print:

```
called: 42
result: 42
```

However, if we destruct the future

When dealing with futures there are three components. The task (or promise) that is executed to resolve the future, the executor (or launch policy) that invokes the task, and the future itself.

Both the form of the task and guarantees provided by the executor impact the capabilities of the future.

For `future::get` and `future::wait` this is particularly problematic because there are situations where calling either of these can result in a deadlock. Specifically, if the task is scheduled for execution on the calling thread, and the task cannot be promoted to immediate execution, then a deadlock is possible. This is true even if the task is scheduled in a thread pool which supports task stealing.

Why would a task not be able to be promoted? There are two reasons: First, in order to be promoted a task has be _resolved_ into a nullary function. For example a `packaged_task<int(int)>` cannot be promoted for immediate execution because it requires an `int` argument. Second, if the task is required to be serialized with respect to other tasks, such as a serial queue executor, then promoting the task to immediate would potentially violate the task ordering.

Currently, in C++, the only way to get a promotable `std::future` is to use `async()` with `launch::deferred`. However, we can leverage that to
