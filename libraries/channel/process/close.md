---
layout: method
title: process::close
tags: [library]
full-name: process::close
pure-name: close
declaration: 
description: This method is called on an await-process whenever the process state is `await_forever` and the incoming queue went dry. As well it is called when an exception is thrown while calling `await` or `yield` and no `set_error()`is available. 
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