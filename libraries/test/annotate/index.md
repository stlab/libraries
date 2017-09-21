---
layout: class
title: stlab::annotate
tags: [library]
name: stlab::annotate
pure-name: annotate
defined-in-header: stlab/test/model.hpp
ctor: default
dtor: default
annotation: template class
declaration: class annotate
brief: A class used to illustrate compiler and object behavior
example: annotate_rvo_example.cpp
description: |
    Annotate will write to `std::cout` when the following events happen to an instance:

    - ctor
    - dtor
    - copy-ctor
    - move-ctor
    - copy-assign
    - move-assign
    - swap

    In addition, `operator==` is always `true`, and `operator!=` is always `false`.
---
