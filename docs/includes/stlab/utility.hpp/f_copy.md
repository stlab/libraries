---
layout: function
title: copy
hyde:
  owner: sean-parent
  brief: Returns a copy of the argument
  tags:
    - function
  inline:
    brief:
      - Returns a copy of the argument. Used to pass an lvalue to function taking an rvalue or to copy a type with an `explicit` copy-constructor.
  defined_in_file: stlab/utility.hpp
  overloads:
    "template <typename T>\nconstexpr auto copy(T &&) -> std::decay_t<T>":
      arguments:
        - description: __OPTIONAL__
          name: value
          type: T &&
      description: __INLINED__
      inline:
        description:
          - Returns a copy of the argument. Used to pass an lvalue to function taking an rvalue or to copy a type with an `explicit` copy-constructor.
      return: __OPTIONAL__
      signature_with_names: "template <typename T>\nconstexpr auto copy(T && value) -> std::decay_t<T>"
  namespace:
    - stlab
    - v3
---

Useful for functions that only take an rvalue reference or that have an explicit copy constructor.

Example:

```cpp
void f(vector<int>&&);
//...
vector<int> c = ...;
f(copy(c)); // pass a copy of `c`
f(move(c)); // sink `c`
```
