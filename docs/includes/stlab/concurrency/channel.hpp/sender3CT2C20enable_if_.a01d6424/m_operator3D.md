---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - method
  inline:
    brief:
      - copy-assign uses copy-ctor to keep sender count correct.
  defined_in_file: stlab/concurrency/channel.hpp
  overloads:
    auto operator=(const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &) -> sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
      description: __INLINED__
      inline:
        description:
          - copy-assign uses copy-ctor to keep sender count correct.
      return: __OPTIONAL__
      signature_with_names: auto operator=(const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & x) -> sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
    auto operator=(sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &&) -> sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &:
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto operator=(sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &&) -> sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
---
