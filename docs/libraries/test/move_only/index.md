---
layout: class
title: stlab::move_only
tags: [library]
name: stlab::move_only
pure-name: move_only
defined-in-header: stlab/test/model.hpp
ctor: default
dtor: default
annotation: class
declaration: class move_only
brief: A class used to illustrate and test move-only semantics
example: move_only_rvo_example.cpp
description: |
    A `move_only` instance is limited to move-only assignment and construction.
    It contains an integer that can be used for identification and is read with
    the `member()` function.
---
