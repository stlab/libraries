---
layout: method
title: set_error
tags: [library]
pure-name: set_error
declaration: 
brief: Called if either on calling `await` or `yield` of the upstream process an exception was thrown.
description: This optional method is called if either on calling `await` or `yield` on the upstream process an exception was thrown. The pointer of the caught exception is passed. In case that the process does not provide this method, `close`is called instead of.
So a process should try to handle its errors within itself. Only errors which occur while passing arguments to the process or retrieving from the process are caught here and then passed to the next downstream process, because this process is not able to handle them.
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
        description: The exception pointer of the occurred error
  - kind: result
---
