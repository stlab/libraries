---
layout: method
title: receiver<T>
hyde:
  owner: FelixPetriconi
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/channel.hpp
  is_ctor: true
  overloads:
    receiver<T>():
      annotation:
        - defaulted
      description: __OPTIONAL__
      signature_with_names: receiver<T>()
    receiver<T>(const receiver<T> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const receiver<T> &
      description: __MISSING__
      signature_with_names: receiver<T>(const receiver<T> & x)
    receiver<T>(receiver<T> &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: receiver<T> &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: receiver<T>(receiver<T> &&)
---
