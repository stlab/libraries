---
layout: function
title: reverse
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - Range-based `reverse`, will be deprecated in C++20 in favor of [`std::ranges::reverse`](https://en.cppreference.com/w/cpp/algorithm/ranges/reverse).
  defined_in_file: stlab/algorithm/reverse.hpp
  overloads:
    "template <class BidirectionalRange>\nvoid reverse(BidirectionalRange &)":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: BidirectionalRange &
      description: __INLINED__
      inline:
        description:
          - Range-based `reverse`, will be deprecated in C++20 in favor of [`std::ranges::reverse`](https://en.cppreference.com/w/cpp/algorithm/ranges/reverse).
      return: __OPTIONAL__
      signature_with_names: "template <class BidirectionalRange>\nvoid reverse(BidirectionalRange & range)"
  namespace:
    - stlab
---
