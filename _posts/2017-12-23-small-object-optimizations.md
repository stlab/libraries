---
title: Small Object Optimization for Polymorphic Types
layout: post
tags: [tips]
comments: true
categories: tip
---

At [Meeting C++ 2017](http://meetingcpp.com/2017/) I presented a lightning talk [Polymorphic Task Template in Ten](http://sean-parent.stlab.cc/papers-and-presentations#polymorphic-task-template-in-ten) which showed an easy way to implement a polymorphic task template, similar to [`std::function`](http://en.cppreference.com/w/cpp/utility/functional/function), with a small object optimization in 10 minutes.

Unfortunately, the small object optimization, as described, leads to [_undefined behavior_](http://en.cppreference.com/w/cpp/language/ub). As a general rule, you cannot store an object of one type in memory and read it back as another, even if the other type is the objects base class (there are exceptions to this rule, but they do not apply to this situation).

This tip looks at the details of the issue and describes a solution which turns out to be more efficient although it does require slightly more, and more complex, code to implement. A small object optimization for polymorphic types is a very useful construct so it important to understand how it can be done correctly.

<!--more-->

## The Problem

If you haven't seen [the talk](https://www.youtube.com/watch?v=2KGkcGtGVM4), I recommend watching it before reading further.

Maikel Nadolski pointed out in the #meetingcpp Slack channel that this solution relies on undefined behavior because it reads back an object as a different type than what was stored.

From the example in the talk, here is the issue:

The constructor for the task is using placement new on a `model` type in the aligned storage member, `_data` .

```cpp
template <class F>
task(F&& f) {
    constexpr bool is_small = sizeof(model<decay_t<F>, true>) <= small_size;
    new (&_data) model<decay_t<F>, is_small>(forward<F>(f));
}
```

The code to access the `model` does so by casting the address of `_data` to `concept`, a base class of `model`.

```cpp
concept& self() { return *static_cast<concept*>(static_cast<void*>(&_data)); }
```

Since a `concept` was not stored at this location, this cast is only allowed if the pointer types `model*` and `concept*` are [_pointer-interconvertible_](http://en.cppreference.com/w/cpp/language/static_cast). Pointer-interconvertible means that a void* which points to an object of one type, also points to an object of the other type. That would only be true if `model` is a [_standard-layout class_](http://en.cppreference.com/w/cpp/named_req/StandardLayoutType). Unfortunately, that is not the case for two reasons.

- `model` (and `concept`) contain virtual functions.
- `model` may have a member which is not a `StandardLayoutType` because it holds an arbitrary `Callable` object.

Because of this, the cast to `concept` is undefined behavior.

## The Fix

One possible fix is to store the pointer returned by placement new, cast to the base `concept*`. I believe this is the approach taken by most standard library implementations. This costs an extra word of storage, and an extra indirection on every use, and it will _always_ (as far as I can determine) be pointing to the aligned storage (a fixed offset), but it is not guaranteed to point directly to the aligned storage.

I could lobby the standard committee to loosen the requirements on pointer-interconvertible. As one example; the class [`adobe::any_regular_t`](https://github.com/stlab/adobe_source_libraries/blob/master/adobe/any_regular.hpp), which has been in fairly wide use for over a decade, suffers the same problem when the captured value is not a `StandardLayoutType`. I believe the standard should ratify existing practice, but this is a very complex area so I will leave it to those who have a deeper understanding to debate.

There is a trick used in `any_regular_t` to remove the need for virtual member functions which can be used to fix the task implementation (and to fix `any_regular_t`). `any_regular_t` still uses inheritance, but it avoids language virtual functions by building [vtables](https://en.wikipedia.org/wiki/Virtual_method_table) with templates. This was done to guarantee object layout for ABI compatibility. To have a complete solution, we also need to remove inheritance. To do that, we split the vtable pointer for our pure virtual base class from the remainder of the object. Seeing how this is done may be very instructive if you are not familiar with how vtables work.

Using the terminology from my talk, I recast the idea of the `concept` class into a struct holding function pointers for the virtual operations. The definition of `task::concept` becomes:

```cpp
template <class R, class... Args>
struct task<R(Args...)>::concept {
    void (*_dtor)(void*) noexcept;
    void (*_move)(void*, void*) noexcept;
    R (*_invoke)(void*, Args&&...);
};
```

The first argument to each function pointer will be the _this_ pointer. The `_dtor` and `_move` functions are used to implement the destructor and both the move constructor and move assignment operators. The `_invoke` function is used to invoke the type erased `Callable`.

The task stores a `const concept*`, simply called `_concept` (this is our vtable pointer), as well as aligned storage for the model. The `_concept` member is initialized to point to a `concept` with a do nothing `_dtor` function. This is the equivalent of declaring a pure base class as:

```cpp
struct concept {
    ~concept() = default;
    virtual move(void*) noexcept = 0;
    virtual R invoke(Args&&...) = 0;
};
```

But since we can't use inheritance or virtual functions, we write this as:

```cpp
    static constexpr concept empty{[](void*) noexcept {}}; // vtable
    const concept* _concept = &empty; // vtable pointer
```

To implement a model declare a set of static functions, which will act as virtual functions, and use them to populate the vtable. The static functions can cast the first argument to the model type safely (the _this_ pointer) and then access any members in the model. Our small model looks like this:

```cpp
template <class R, class... Args>
template <class F>
struct task<R(Args...)>::model<F, true> {
    template <class G>
    model(G&& f) : _f(forward<G>(f)) {}

    static void _dtor(void* self) noexcept { static_cast<model*>(self)->~model(); }
    static void _move(void* self, void* p) noexcept {
        new (p) model(move(*static_cast<model*>(self)));
    }
    static R _invoke(void* self, Args&&... args) {
        return invoke(static_cast<model*>(self)->_f, forward<Args>(args)...);
    }

    static constexpr concept vtable{_dtor, _move, _invoke};

    F _f;
};
```

And the large model (where the `Callable` is held by a `unique_ptr`) looks like this:

```cpp
template <class R, class... Args>
template <class F>
struct task<R(Args...)>::model<F, false> {
    template <class G>
    model(G&& f) : _p(make_unique<F>(forward<F>(f))) {}

    static void _dtor(void* self) noexcept { static_cast<model*>(self)->~model(); }
    static void _move(void* self, void* p) noexcept {
        new (p) model(move(*static_cast<model*>(self)));
    }
    static R _invoke(void* self, Args&&... args) {
        return invoke(*static_cast<model*>(self)->_p, forward<Args>(args)...);
    }

    static constexpr concept vtable{_dtor, _move, _invoke};

    unique_ptr<F> _p;
};
```

It is straightforward to fill in the basic operations on the task in terms of model operations:

```cpp
template <class R, class... Args>
class task<R(Args...)> {
    struct concept;

    template <class F, bool Small>
    struct model;
    static constexpr size_t small_size = sizeof(void*) * 4;

    static constexpr concept empty{[](void*) noexcept {}};
    const concept* _concept = &empty;
    aligned_storage_t<small_size> _model;

public:
    template <class F>
    task(F&& f) {
        constexpr bool is_small = sizeof(model<decay_t<F>, true>) <= small_size;
        new (&_model) model<decay_t<F>, is_small>(forward<F>(f));
        _concept = &model<decay_t<F>, is_small>::vtable;
    }

    ~task() { _concept->_dtor(&_model); }

    task(task&& x) noexcept : _concept(x._concept) { _concept->_move(&x._model, &_model); }
    task& operator=(task&& x) noexcept {
        if (this == &x) return *this;
        _concept->_dtor(&_model);
        _concept = x._concept;
        _concept->_move(&x._model, &_model);
        return *this;
    }
    R operator()(Args... args) { return _concept->_invoke(&_model, forward<Args>(args)...); }
};
```

The `task` never casts to either a `concept` or a `model` pointer, instead a pointer to the `_model` storage is passed to a function in the vtable which casts the pointer to the correct model type stored in that memory. Undefined behavior is avoided because the pointer is only ever cast to the stored type.

Using this technique generates less code, largely because without virtual functions there are no tables for RTTI generated. But even looking beyond that the code appears to be slightly more optimal (if anyone wants to construct a good benchmark, post a link in the comments). Here is the [original code](https://godbolt.org/g/ki2TSJ), with undefined behavior, presented at my talk and here is the [code without inheritance or language virtual functions](https://godbolt.org/g/oxBz9T).

I'm in the process of updating the [stlab library implementation of tasks](https://github.com/stlab/libraries/pull/118). It hasn't been decided if we roll this into a 1.1.2 bug fix release or hold it for a 1.2 release. I'm also looking at fixing `adobe::any_regular_t` in ASL.

I'll update my lighting talk if I get asked to present it again. This solution is a little more complicated but it still demonstrates the power of using concept based polymorphism to shift the allocation strategy to the use of the objects.

As part of preparing this errata, Eric Niebler informed me that his [folly/Poly library](https://github.com/facebook/folly/blob/master/folly/docs/Poly.md) uses a similar technique to implement the small object optimization.

I frequently tell developers that writing papers and presenting talks is a really good way to improve your code. It can be a bit embarrassing when you present a great way to use undefined behavior at a major conference, but this is how we learn and grow as professionals. Thanks Maikel for the catch!
