---
layout: function
title: operator&
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  detail::annotations operator&(const stlab::executor &, stlab::buffer_size):
    arguments:
      - description: __OPTIONAL__
        name: e
        type: const stlab::executor &
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: detail::annotations operator&(const stlab::executor & e, stlab::buffer_size bs)
  detail::annotations operator&(stlab::buffer_size, const stlab::executor &):
    arguments:
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
      - description: __OPTIONAL__
        name: e
        type: const stlab::executor &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: detail::annotations operator&(stlab::buffer_size bs, const stlab::executor & e)
  detail::annotations operator&(stlab::buffer_size, stlab::executor &&):
    arguments:
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
      - description: __OPTIONAL__
        name: e
        type: stlab::executor &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: detail::annotations operator&(stlab::buffer_size bs, stlab::executor && e)
  detail::annotations operator&(stlab::executor &&, stlab::buffer_size):
    arguments:
      - description: __OPTIONAL__
        name: e
        type: stlab::executor &&
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: detail::annotations operator&(stlab::executor && e, stlab::buffer_size bs)
  "template <typename F>\ndetail::annotated_process<F> operator&(F &&, detail::annotations &&)":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
      - description: __OPTIONAL__
        name: a
        type: detail::annotations &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(F && f, detail::annotations && a)"
  "template <typename F>\ndetail::annotated_process<F> operator&(F &&, stlab::buffer_size)":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(F && f, stlab::buffer_size bs)"
  "template <typename F>\ndetail::annotated_process<F> operator&(detail::annotated_process<F> &&, stlab::buffer_size)":
    arguments:
      - description: __OPTIONAL__
        name: a
        type: detail::annotated_process<F> &&
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(detail::annotated_process<F> && a, stlab::buffer_size bs)"
  "template <typename F>\ndetail::annotated_process<F> operator&(detail::annotated_process<F> &&, stlab::executor &&)":
    arguments:
      - description: __OPTIONAL__
        name: a
        type: detail::annotated_process<F> &&
      - description: __OPTIONAL__
        name: e
        type: stlab::executor &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(detail::annotated_process<F> && a, stlab::executor && e)"
  "template <typename F>\ndetail::annotated_process<F> operator&(detail::annotations &&, F &&)":
    arguments:
      - description: __OPTIONAL__
        name: a
        type: detail::annotations &&
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(detail::annotations && a, F && f)"
  "template <typename F>\ndetail::annotated_process<F> operator&(executor_task_pair<F> &&, stlab::buffer_size)":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F> &&
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(executor_task_pair<F> && etp, stlab::buffer_size bs)"
  "template <typename F>\ndetail::annotated_process<F> operator&(stlab::buffer_size, F &&)":
    arguments:
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(stlab::buffer_size bs, F && f)"
  "template <typename F>\ndetail::annotated_process<F> operator&(stlab::buffer_size, executor_task_pair<F> &&)":
    arguments:
      - description: __OPTIONAL__
        name: bs
        type: stlab::buffer_size
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F> &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\ndetail::annotated_process<F> operator&(stlab::buffer_size bs, executor_task_pair<F> && etp)"
namespace:
  - stlab
  - v1
---
