---
layout: function
title: find_parent
owner: fosterbrereton
brief: Get the parent node of a child node
tags:
  - function
defined_in_file: forest.hpp
overloads:
  "template <class I>\nI find_parent(I)":
    arguments:
      - description: __OPTIONAL__
        name: i
        type: I
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class I>\nI find_parent(I i)"
namespace:
  - stlab
---

The time complexity for this operation is $O(N)$, where $N$ is the number of child nodes of the parent.
