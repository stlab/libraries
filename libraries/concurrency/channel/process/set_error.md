---
layout: method
title: set_error
tags: [library]
pure-name: set_error
declaration: 
brief: Called if either on calling `await` or `yield` an exception was thrown
description: This optional method is called if either on calling `await` or `yield` an exception was thrown. The pointer of the caught exception is passed. In case that the process does not provide this method, `close`is called instead of.
entities:
  - kind: methods
    list:
      - name: process::set_error
        pure-name: set_error
        declaration: void set_error(std::exception_ptr error)
        description: This optional method is called
  - kind: parameters
    list:
      - name: error
        description: The exception pointer of the occured error
  - kind: result
---
