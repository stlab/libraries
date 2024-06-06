---
layout: function
title: reverse_copy
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/algorithm/reverse.hpp
  overloads:
    "template <class BidirectionalRange, class OutputIterator>\nvoid reverse_copy(BidirectionalRange &, OutputIterator)":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: BidirectionalRange &
        - description: __OPTIONAL__
          name: result
          type: OutputIterator
      description: __INLINED__
      inline:
        description:
          - Range-based `reverse_copy` algorithm, will be deprecated in C++20 in favor of [`std::ranges::reverse_copy`](https://en.cppreference.com/w/cpp/algorithm/ranges/reverse_copy).
      return: __OPTIONAL__
      signature_with_names: "template <class BidirectionalRange, class OutputIterator>\nvoid reverse_copy(BidirectionalRange & range, OutputIterator result)"
    "template <class BidirectionalRange, class OutputIterator>\nvoid reverse_copy(const BidirectionalRange &, OutputIterator)":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: const BidirectionalRange &
        - description: __OPTIONAL__
          name: result
          type: OutputIterator
      description: __INLINED__
      inline:
        description:
          - Range-based `reverse_copy` algorithm, will be deprecated in C++20 in favor of [`std::ranges::reverse_copy`](https://en.cppreference.com/w/cpp/algorithm/ranges/reverse_copy).
      return: __OPTIONAL__
      signature_with_names: "template <class BidirectionalRange, class OutputIterator>\nvoid reverse_copy(const BidirectionalRange & range, OutputIterator result)"
  namespace:
    - stlab
---
