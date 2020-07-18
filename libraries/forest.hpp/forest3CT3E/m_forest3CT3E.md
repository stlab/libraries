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
        name: unnamed-0
        type: const forest<T> &
        unnamed: true
    description: Copy ctor
    return: __OPTIONAL__
    signature_with_names: forest<T>(const forest<T> &)
  forest<T>(forest<T> &&):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: forest<T> &&
        unnamed: true
    description: Move ctor
    return: __OPTIONAL__
    signature_with_names: forest<T>(forest<T> &&)
---
