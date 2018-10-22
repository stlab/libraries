---
layout: method
title: model
owner: sparent
brief: Constructor
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
is_ctor: true
overloads:
  model():
    annotation:
      - public
      - default
    description: Default constructor
    return: __OPTIONAL__
    signature_with_names: model()
  template <class... Args> model(Args &&...):
    annotation:
      - public
    arguments:
      - description: __OPTIONAL__
        name: args
        type: Args &&...
    description: Variadic constructor
    return: __OPTIONAL__
    signature_with_names: template <class... Args> model(Args &&... args)
---
