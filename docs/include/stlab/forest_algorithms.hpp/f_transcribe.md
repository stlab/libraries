---
layout: function
title: transcribe
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest_algorithms.hpp
  overloads:
    "template <class I, class O, class P, class UP>\nauto transcribe(I, const I &, O, P, UP)":
      arguments:
        - description: __OPTIONAL__
          name: first
          type: I
        - description: __OPTIONAL__
          name: last
          type: const I &
        - description: __OPTIONAL__
          name: out
          type: O
        - description: __OPTIONAL__
          name: proj
          type: P
        - description: __OPTIONAL__
          name: pred
          type: UP
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class I, class O, class P, class UP>\nauto transcribe(I first, const I & last, O out, P proj, UP pred)"
    "template <class I, class O, class P>\nauto transcribe(const I &, const I &, O, P)":
      arguments:
        - description: __OPTIONAL__
          name: first
          type: const I &
        - description: __OPTIONAL__
          name: last
          type: const I &
        - description: __OPTIONAL__
          name: out
          type: O
        - description: __OPTIONAL__
          name: proj
          type: P
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class I, class O, class P>\nauto transcribe(const I & first, const I & last, O out, P proj)"
    "template <class R, class O, class P, class UP>\nauto transcribe(const R &, O, P, UP)":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: const R &
        - description: __OPTIONAL__
          name: out
          type: O
        - description: __OPTIONAL__
          name: proj
          type: P
        - description: __OPTIONAL__
          name: pred
          type: UP
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class R, class O, class P, class UP>\nauto transcribe(const R & range, O out, P proj, UP pred)"
    "template <class R, class O, class P>\nauto transcribe(const R &, O, P)":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: const R &
        - description: __OPTIONAL__
          name: out
          type: O
        - description: __OPTIONAL__
          name: proj
          type: P
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class R, class O, class P>\nauto transcribe(const R & range, O out, P proj)"
  namespace:
    - stlab
    - forests
---
