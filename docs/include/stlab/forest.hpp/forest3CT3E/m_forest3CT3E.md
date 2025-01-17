---
layout: method
title: forest<T>
hyde:
  owner: fosterbrereton
  brief: Constructor
  tags:
    - method
  defined_in_file: stlab/forest.hpp
  is_ctor: true
  overloads:
    forest<T>():
      annotation:
        - defaulted
      description: Default ctor
      signature_with_names: forest<T>()
    forest<T>(const forest<T> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const forest<T> &
      description: Copy ctor
      signature_with_names: forest<T>(const forest<T> & x)
    forest<T>(forest<T> &&):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: forest<T> &&
      description: Move ctor
      signature_with_names: forest<T>(forest<T> && x)
---
