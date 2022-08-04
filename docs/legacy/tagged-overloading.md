---
title: Tagged Overloading
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Tagged_Overloading
---

The problem of being able to refine algorithms and how the techniques for doing so interact with namespaces is an ongoing problem for us with C++. Within ASL (and Boost) there are several techniques which are used, all with various drawbacks.

The issue here is that for a refinement to work, it needs to find the most specific implementation at the point of instantiation. A qualified name in C++ is bound at the point of definition, and although you can have complete specializations which will be found at instatiation time, overload resolution will only happen at definition time.

The same is not true for unqualified names. For unqualified names, final overload resolution happens at instantiation time using ADL. The problem here, is that namespaces are defeated by ADL, meaning you could find a function accidentally which didn't have the proper semantics (only a matching signature) and you can also have conflicting functions.

There are currently two main techniques used:
The first is to shim the function to call through a function object, and the client can fully or partially specialize the function object. The syntax for this is fairly verbose:

```cpp
#include <iostream>
#include <iterator>

namespace adobe {

namespace fn { } using namespace fn;

template <typename I, typename T>
struct advance_t
{
    void operator()(I& iter, T n)
    {
    while (n--) ++iter;
    std::cout << "called adobe::advance" << std::endl;
    }
};

namespace fn {

template <typename I, typename T> // I models InputIterator
void advance(I& iter, T n)
{
    adobe::advance_t<I, T>()(iter, n);
}

} // namespace fn

} // namespace adobe

// User Code

namespace {

class my_type { };

} // namespace

namespace adobe {

template <>
struct advance_t<my_type*, int>
{
    void operator()(my_type*& iter, int n)
    {
    iter += n;
    std::cout << "called user specializtion" << std::endl;
    }

};

} // namespace adobe

int main (int argc, char * const argv[])
{
    my_type* x = NULL;
    adobe::advance(x, 5);

    return 0;
}
```

This works well, the problem is the user code is very cumbersome to write. As mentioned, another approach is to use ADL - but then we risk pulling in the wrong function. Below is an augmented form of the ADL solution, here we provide an additional parameter tag "adobe::overload" - this tag serves to qualify the call as having the same semantics as what was meant by the advance() function defined in the adobe namespace. The client can easily overload (including using templates) within their namespace and without the heavy syntax of template specialization.

```cpp
#include <iostream>
#include <iterator>

namespace adobe {
namespace fn { } using namespace fn;

class overload { };

namespace fn {

// In this case we will provide an Adobe "default" version of advance so
// that the client does not need to provide one at all. This has two
// tradeoffs. The first (obvious) tradeoff is that the client will not
// receive a natural compilation error in case they forgot to provide an
// intended overload (nor can this be helped with boost::concept). The
// second tradeoff is that the "default" version of advance must preceed
// the two argument version or else a compilation error will occur. This
// is because the non-ADL part of the unqualified lookup occurs at the
// point of definition, not at the point of instantiation, even though
// a dependent name is involved. The relevant portion of the standard for
// this (possibly counterintuitive) rule is 14.6.4.2

template <typename I, typename T> // I models InputIterator
void advance(I& iter, T n, adobe::overload)
{
    while (n--) ++iter;
    std::cout << "called adobe::advance" << std::endl;
}

// The user can replace the above advance with an overload or a
// specialization. If using an overload, a user-defined type must be
// involved and the overload must live in the same namespace as one
// of the user defined types, or else it must be declared before
// the advance following this comment. This is slightly brittle
// but we consider it workable for now.

template <typename I, typename T> // I models InputIterator
void advance(I& iter, T n)
{
    advance(iter, n, adobe::overload());
}

} // namespace fn

} // namespace adobe

// User Code

namespace {

class my_type { };

void advance(my_type* iter, int n, adobe::overload)
{
    iter += n;
    std::cout << "called user advance" << std::endl;
}

} // namespace

int main (int argc, char * const argv[])
{
    my_type* x = NULL;
    adobe::advance(x, 5);

    return 0;
}
```
