---
layout: method
title: copy_on_write<T>
owner: sean-parent
brief: Constructor
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
is_ctor: true
overloads:
  copy_on_write<T>():
    description: Default constructor
    return: __OPTIONAL__
    signature_with_names: copy_on_write<T>()
  copy_on_write<T>(const copy_on_write<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
    description: Copy constructor
    return: __OPTIONAL__
    signature_with_names: copy_on_write<T>(const copy_on_write<T> & x)
  copy_on_write<T>(copy_on_write<T> &&):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: copy_on_write<T> &&
    description: Move constructor
    return: __OPTIONAL__
    signature_with_names: copy_on_write<T>(copy_on_write<T> && x)
  "template <class U, class V, class... Args>\ncopy_on_write<T>(U &&, V &&, Args &&...)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: U &&
      - description: __OPTIONAL__
        name: y
        type: V &&
      - description: __OPTIONAL__
        name: args
        type: Args &&...
    description: Constructor
    return: __OPTIONAL__
    signature_with_names: "template <class U, class V, class... Args>\ncopy_on_write<T>(U && x, V && y, Args &&... args)"
  "template <class U>\ncopy_on_write<T>(U &&, disable_copy<U>)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: U &&
      - description: __OPTIONAL__
        name: unnamed-1
        type: disable_copy<U>
        unnamed: true
    description: Constructor
    return: __OPTIONAL__
    signature_with_names: "template <class U>\ncopy_on_write<T>(U && x, disable_copy<U>)"
---
