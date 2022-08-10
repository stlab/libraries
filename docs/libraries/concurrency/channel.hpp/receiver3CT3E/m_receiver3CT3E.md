---
layout: method
title: receiver<T>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/channel.hpp
is_ctor: true
overloads:
  receiver<T>():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: receiver<T>()
  receiver<T>(const receiver<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const receiver<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: receiver<T>(const receiver<T> & x)
  receiver<T>(receiver<T> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: receiver<T> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: receiver<T>(receiver<T> &&)
---
