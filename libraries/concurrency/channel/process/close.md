---
layout: function
title: close
brief: Called on an await-process whenever the process state is `await_forever` and the incoming queue went dry.
tags:
  - function
overloads:
  void close():
    description: This method signals the await-process that either the queue of incoming values went dry or an error happened.
    return: __OPTIONAL__
    signature_with_names: void close()
---
This optional method is called on an await-process whenever the process state is `await_forever` and the incoming queue went dry. It is also called when an exception upstream is thrown while calling `await` or `yield` and here no `set_error()` is available. After a `close()`, your process will continue to run so long as your `state()` function returns `yield`. When your `state()` goes to `await`, the process will terminate.