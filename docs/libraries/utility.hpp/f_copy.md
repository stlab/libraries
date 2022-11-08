---
layout: function
title: copy
owner: sean-parent
brief: Returns a copy of the argument
tags:
  - function
defined_in_file: utility.hpp
overloads:
  "template <typename T>\nT copy(const T &)":
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const T &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nT copy(const T & a)"
namespace:
  - stlab
  - v1
---

Useful for functions that only take an rvalue reference or that have an explicit copy constructor.

Example:
```cpp
void f(vector<int>&&);
//...
vector<int> a = ...;
f(copy(a)); // pass a copy
f(move(a)); // sink a
```
