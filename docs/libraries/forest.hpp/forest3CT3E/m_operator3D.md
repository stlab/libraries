---
layout: method
title: operator=
owner: fosterbrereton
brief: Assignment operator
tags:
  - method
defined_in_file: forest.hpp
overloads:
  forest<T> & operator=(const forest<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const forest<T> &
    description: Copy assignment
    return: __OPTIONAL__
    signature_with_names: forest<T> & operator=(const forest<T> & x)
  forest<T> & operator=(forest<T> &&):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: forest<T> &&
    description: Move assignment
    return: __OPTIONAL__
    signature_with_names: forest<T> & operator=(forest<T> && x)
---
