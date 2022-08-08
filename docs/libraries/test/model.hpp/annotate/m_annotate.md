---
layout: method
title: annotate
owner: sean-parent
brief: Constructs an annotate
tags:
  - method
defined_in_file: test/model.hpp
is_ctor: true
overloads:
  annotate(const stlab::annotate &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const stlab::annotate &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: annotate(const stlab::annotate & x)
  annotate(stlab::annotate &&):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: stlab::annotate &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: annotate(stlab::annotate && x)
  explicit annotate(stlab::annotate_counters &):
    arguments:
      - description: The instance of counters in which all events are recorded
        name: counters
        type: stlab::annotate_counters &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: explicit annotate(stlab::annotate_counters & counters)
---
