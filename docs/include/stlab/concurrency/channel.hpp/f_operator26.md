---
layout: function
title: operator&
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/channel.hpp
  overloads:
    auto operator&(const stlab::executor &, stlab::buffer_size) -> detail::annotations:
      arguments:
        - description: __OPTIONAL__
          name: e
          type: const stlab::executor &
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator&(const stlab::executor & e, stlab::buffer_size bs) -> detail::annotations
    auto operator&(stlab::buffer_size, const stlab::executor &) -> detail::annotations:
      arguments:
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
        - description: __OPTIONAL__
          name: e
          type: const stlab::executor &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator&(stlab::buffer_size bs, const stlab::executor & e) -> detail::annotations
    auto operator&(stlab::buffer_size, stlab::executor &&) -> detail::annotations:
      arguments:
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
        - description: __OPTIONAL__
          name: e
          type: stlab::executor &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator&(stlab::buffer_size bs, stlab::executor && e) -> detail::annotations
    auto operator&(stlab::executor &&, stlab::buffer_size) -> detail::annotations:
      arguments:
        - description: __OPTIONAL__
          name: e
          type: stlab::executor &&
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator&(stlab::executor && e, stlab::buffer_size bs) -> detail::annotations
    "template <typename F>\nauto operator&(F &&, detail::annotations &&) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: a
          type: detail::annotations &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(F && f, detail::annotations && a) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(F &&, stlab::buffer_size) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(F && f, stlab::buffer_size bs) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(detail::annotated_process<F> &&, stlab::buffer_size) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: a
          type: detail::annotated_process<F> &&
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(detail::annotated_process<F> && a, stlab::buffer_size bs) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(detail::annotated_process<F> &&, stlab::executor &&) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: a
          type: detail::annotated_process<F> &&
        - description: __OPTIONAL__
          name: e
          type: stlab::executor &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(detail::annotated_process<F> && a, stlab::executor && e) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(detail::annotations &&, F &&) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: a
          type: detail::annotations &&
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(detail::annotations && a, F && f) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(executor_task_pair<F> &&, stlab::buffer_size) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: etp
          type: executor_task_pair<F> &&
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(executor_task_pair<F> && etp, stlab::buffer_size bs) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(stlab::buffer_size, F &&) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(stlab::buffer_size bs, F && f) -> detail::annotated_process<F>"
    "template <typename F>\nauto operator&(stlab::buffer_size, executor_task_pair<F> &&) -> detail::annotated_process<F>":
      arguments:
        - description: __OPTIONAL__
          name: bs
          type: stlab::buffer_size
        - description: __OPTIONAL__
          name: etp
          type: executor_task_pair<F> &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(stlab::buffer_size bs, executor_task_pair<F> && etp) -> detail::annotated_process<F>"
  namespace:
    - stlab
    - v3
---
