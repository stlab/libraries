---
layout: function
title: operator!=
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/copy_on_write.hpp
  overloads:
    auto operator!=(const copy_on_write<T> &, const copy_on_write<T> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const copy_on_write<T> &
        - description: __OPTIONAL__
          name: y
          type: const copy_on_write<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const copy_on_write<T> & x, const copy_on_write<T> & y) -> bool
    auto operator!=(const copy_on_write<T> &, const stlab::copy_on_write::element_type &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const copy_on_write<T> &
        - description: __OPTIONAL__
          name: y
          type: const stlab::copy_on_write::element_type &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const copy_on_write<T> & x, const stlab::copy_on_write::element_type & y) -> bool
    auto operator!=(const stlab::copy_on_write::element_type &, const copy_on_write<T> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const stlab::copy_on_write::element_type &
        - description: __OPTIONAL__
          name: y
          type: const copy_on_write<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const stlab::copy_on_write::element_type & x, const copy_on_write<T> & y) -> bool
  namespace:
    - stlab
---
