---
layout: function
title: set_error
brief: Called if either on calling `await` or `yield` of the upstream process an exception was thrown.
tags:
  - function
overloads:
  void set_error(std::exception_ptr ex):
    arguments:
      - description: The exception pointer of the occurred error
        name: ex
        type: std::exception_ptr
    return: __OPTIONAL__
    signature_with_names: void set_error(std::exception_ptr error)
---
This optional method is called if either on calling `await` or `yield` on the upstream process an exception was thrown. The pointer of the caught exception is passed. In case that the process does not provide this method, `close`is called instead of. So a process should try to handle its errors within itself. Only errors which occur while passing arguments to the process or retrieving from the process are caught here and then passed to the next downstream process, because this process is not able to handle them.
