---
layout: method
title: stlab::copy_on_write::write
tags: [library]
scope: copy_on_write
pure-name: write
defined-in-header: stlab/copy_on_write.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/copy_on_write.hpp
brief: Obtain a reference to the value the object is referencing.
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::write
        pure-name: write
        declaration: |
            auto write() -> element_type&
        description: Nonconst accessor
  - kind: result
    description: A reference to the underlying object
---

Obtain a reference to the value the object is referencing. This will copy the underlying value (if necessary) so changes to the value do not affect other `copy_on_write` objects.

Note that `write()` does not have the same preconditions as `operator=()`. `write()` returns a reference to the underlying object's value, thus requiring that an underlying object exist. `operator=()` on the other hand will perform an allocation if one is necessary.
