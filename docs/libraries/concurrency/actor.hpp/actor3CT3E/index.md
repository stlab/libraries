---
layout: class
title: actor<T>
owner: fbrereto
brief: Serialized, asynchronous access to a resource
tags:
  - class
defined_in_file: concurrency/actor.hpp
declaration: "template <class T>\nclass stlab::actor;"
dtor: unspecified
fields:
  _impl:
    annotation:
      - private
    description: pimpl implementation instance
    type: std::shared_ptr<detail::actor_instance<T>>
namespace:
  - stlab
---

`actor<T>` provides asynchronous, serialized access to an instance of `T`, running on an execution context of choice. Instead of a traditional message-passing actor model implementation, `actor<T>` is given work by way of lambdas, whose results are then optionally extracted by the caller via a `stlab::future<R>`.

`actor<T>` is a lightweight alternative to a dedicated thread managing some background service for a host application. The problem with background threads is that they consume considerable resources even when they are idle. Furthermore, many background services don't need the "always on" characteristics of a thread, and would be comfortable running only when necessary.

However, `actor<T>` is not a panacea. There are several caveats to keep in mind:

1. `thread_local` variables may not retain state from task to task. Given the implementation details of the actor's executor (e.g., it may be scheduled on any number of threads in a thread pool), an actor may jump from thread to thread. Since `thread_local` variables have a per-thread affinity by definition, the variable values may change unexpectedly.
2. The thread cache penalty paid when an actor changes threads may not be suitable for high-performance/low-latency requirements. There is a cost associated with an actor jumping from one thread to another, and as in the previous case, this may happen depending on the implementation of the executor. If this cache penalty is too expensive for your use case, a dedicated worker thread may be a better fit.
3. The tasks given to an actor should not block. If the actor must wait for external input (mouse events, network/file IO, etc.) it should be fed in from outside the actor. Because the context of execution is not "owned" by the actor, it cannot presume to block the context waiting for something else to happen, or else it risks hanging (e.g., an unresponsive main thread) or deadlocking (e.g., waiting for a task that cannot complete until this task completes.)

## Example

Say we have a service, `type_rasterizer`, that we'd like to put on a background thread:

```c++
class image {
    //...
};

struct type_rasterizer {
    void set_text(std::string&& text);

    image rasterize();

    // ... 
};
```

In our application, then, we will create an actor that manages an instance of this engine. By giving it the `default_executor`, the actor will run on a thread of the OS-provided thread pool (e.g., GCD on macOS/iOS).

```c++
struct my_application {
    stlab::actor<type_rasterizer> _rasterizer(stlab::default_executor,
                                                "app text rasterizer");

    // ... 
};
```

Then as your application is running, you can send "messages" in the form of lambdas to this actor to perform serialized, asynchronous operations. Note the first parameter of the lambda is the `type_rasterizer` itself:

```c++
void my_application::do_rasterize(std::string&& text) {
    _rasterizer.send([_text = std::move(text)](type_rasterizer& rasterizer) mutable {
        // This lambda will execute on the `default_executor`. Note that while in this
        // lambda, the name of the thread will be the name of the actor. In this case,
        // "app text rasterizer".
        rasterizer.set_text(std::move(_text));
        return rasterizer.rasterize();
    }).then(stlab::main_executor, [](image my_rasterized_text){
        draw_image_to_screen(my_rasterized_text);
    }).detach();
}
```

You could also pass the argument to the lambda itself:

```c++
_rasterizer.send([](type_rasterizer& rasterizer, std::string text) {
    rasterizer.set_text(std::move(text));
    return rasterizer.rasterize();
}, std::move(text));
```

Note that the actor is not always running. That is, no threads are blocked on behalf of the actor while it waits for tasks to come in. Rather, the actor only schedules itself to run on its executor when it has work to do. Once the work is completed, the actor relinquishes the thread it is running on back to the executor. In this way, actors are considerably less resource-intensive than a dedicated worker thread to some background service.
