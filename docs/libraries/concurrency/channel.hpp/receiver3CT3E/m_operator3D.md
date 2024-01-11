---
layout: method
title: operator=
hyde:
  owner: FelixPetriconi
  brief: __MISSING__
  tags:
    - method
  defined_in_file: concurrency/channel.hpp
  overloads:
    receiver<T> & operator=(const receiver<T> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const receiver<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: receiver<T> & operator=(const receiver<T> & x)
    receiver<T> & operator=(receiver<T> &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: x
          type: receiver<T> &&
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: receiver<T> & operator=(receiver<T> && x)
---
