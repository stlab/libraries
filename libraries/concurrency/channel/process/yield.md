---
layout: function
title: yield
brief: Called on an await-process when it should yield
tags:
  - function
overloads:
  T yield():
    description: The result of the process.
    return: __OPTIONAL__
    signature_with_names: T yield()
---
This method is called whenever the state has changed from await to yield or a timeout was provided with the recent call to `state()`.
