---
layout: method
title: forest<T>
owner: fosterbrereton
brief: Constructor
tags:
  - method
defined_in_file: forest.hpp
is_ctor: true
overloads:
  forest<T>():
    annotation:
      - default
    description: Default ctor
    return: __OPTIONAL__
    signature_with_names: forest<T>()
  forest<T>(const forest<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const forest<T> &
    description: Copy ctor
    return: __OPTIONAL__
    signature_with_names: forest<T>(const forest<T> & x)
  forest<T>(forest<T> &&):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: forest<T> &&
    description: Move ctor
    return: __OPTIONAL__
    signature_with_names: forest<T>(forest<T> && x)
---
