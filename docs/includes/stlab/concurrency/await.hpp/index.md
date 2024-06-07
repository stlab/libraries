---
layout: library
title: stlab/concurrency/await.hpp
hyde:
  owner: sean-parent
  brief: |
    Await provides utilities to synchronously await the value from a `future` and to notify the default executor that a task is waiting. Blocking calls are discouraged because they may lead to deadlocks or thread explosions.

    There is a good presentation of the issues [here](https://youtu.be/Z86b3Rd09sE).
  tags:
    - sourcefile
  library-type: sourcefile
---
