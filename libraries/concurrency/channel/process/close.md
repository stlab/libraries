---
layout: method
title: close
tags: [library]
pure-name: close
declaration:
brief: Called on an await-process whenever the process state is `await_forever` and the incoming queue went dry.
description: |
    This optional method is called on an await-process whenever the process state
    is `await_forever` and the incoming queue went dry. It is also called when
    an exception is thrown while calling `await` or `yield` and no `set_error()`
    is available. After a `close()`, your process will continue to run so long
    as your `state()` function returns `yield`. When your `state()` goes to
    `await`, the process will terminate.
entities:
  - kind: methods
    list:
      - name: process::close
        pure-name: close
        declaration: void close()
        description: This method signals the await-process that either the queue of incoming values went dry or an error happened.
  - kind: parameters
  - kind: result
---
