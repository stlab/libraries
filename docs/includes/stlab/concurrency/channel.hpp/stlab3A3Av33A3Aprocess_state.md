---
layout: enumeration
title: process_state
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - enumeration
  inline:
    description:
      - "***********************************************************************************************"
      - close on a process is called when a process is in an await state to signal that no more data is coming. In response to a close, a process can switch to a yield state to yield values, otherwise it is destructed. await_try is await if a value is available, otherwise yield (allowing for an interruptible task).
  defined_in_file: stlab/concurrency/channel.hpp
  namespace:
    - stlab
    - v3
  values:
    - description: __MISSING__
      name: await
    - description: __MISSING__
      name: yield
---
