---
layout: method
title: process::yield
tags: [library]
full-name: process::yield
pure-name: await
declaration: T yield()
description: This method is called on an await-process whenever a new value was received from upstream. The type `T` stands here for any semi regular or move-only type. The number of arguments depends on  the number of attached upstream sender. Potential state changes from awaitable to yieldable should happen while this method is invoked.
entities:
  - kind: methods
    list:
      - name: process::yield
        pure-name: yield
        declaration: T yield()
        description: This method is called whenever the state has changed from await to yield or a timeout was provided with the recent call to `state()`.
  - kind: parameters
  - kind: result
    description: The result of the process.
---