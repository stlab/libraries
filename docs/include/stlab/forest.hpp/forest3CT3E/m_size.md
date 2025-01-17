---
layout: method
title: size
hyde:
  owner: fosterbrereton
  brief: Compute the number of nodes
  tags:
    - method
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  overloads:
    auto size() const -> stlab::forest::size_type:
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto size() const -> stlab::forest::size_type
---

If the size of the forest is known (valid), this routine will return in constant time. Otherwise, it will perform a linear walk of the entire forest to derive its size.

Most operations performed on the forest will keep its size valid. Some splicing operations (e.g., splicing a range of child nodes) can happen where the total number of nodes being sliced is unknown. In those cases, the sizes of both the source and destination forests are invalidated.
