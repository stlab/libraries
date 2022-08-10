---
layout: method
title: progress_tracker
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/progress.hpp
is_ctor: true
overloads:
  progress_tracker():
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: progress_tracker()
  progress_tracker(const stlab::progress_tracker &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const stlab::progress_tracker &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: progress_tracker(const stlab::progress_tracker &)
  progress_tracker(std::function<void (size_t, size_t)>):
    arguments:
      - description: __OPTIONAL__
        name: f
        type: std::function<void (size_t, size_t)>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: progress_tracker(std::function<void (size_t, size_t)> f)
  progress_tracker(stlab::progress_tracker &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: stlab::progress_tracker &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: progress_tracker(stlab::progress_tracker &&)
---
