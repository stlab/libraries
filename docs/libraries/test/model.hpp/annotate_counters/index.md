---
layout: class
title: annotate_counters
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: test/model.hpp
declaration: "\nstruct stlab::annotate_counters;"
ctor: unspecified
dtor: unspecified
fields:
  _condition:
    description: __MISSING__
    type: std::condition_variable
  _copy_assign_lhs:
    description: __MISSING__
    type: std::atomic_size_t
  _copy_assign_rhs:
    description: __MISSING__
    type: std::atomic_size_t
  _copy_ctor:
    description: __MISSING__
    type: std::atomic_size_t
  _dtor:
    description: __MISSING__
    type: std::atomic_size_t
  _equality:
    description: __MISSING__
    type: std::atomic_size_t
  _move_assign_lhs:
    description: __MISSING__
    type: std::atomic_size_t
  _move_assign_rhs:
    description: __MISSING__
    type: std::atomic_size_t
  _move_ctor:
    description: __MISSING__
    type: std::atomic_size_t
  _mutex:
    description: __MISSING__
    type: std::mutex
  _swap:
    description: __MISSING__
    type: std::atomic_size_t
namespace:
  - stlab
  - v1
---
