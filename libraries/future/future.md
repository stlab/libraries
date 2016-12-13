---
layout: method
title: stlab::future
entities:
  - kind: method
    name: stlab::future::future
    pure-name: future
    defined-in-header: stlab/future.hpp
    declaration: future()
    member-functions:
      - name: valid
        description: Returns true if the future is valid
      - name: then
        description: Creates a continuation
      - name: recover
        description: Creates a recovering continuation
      - name: detach
        description: Detaches the current future
      - name: cancel_try
        description: Tries to cancel the future
      - name: get_try
        description: Tries to access the futures result
      - name: error
        description: Returns a possibly stores exception
---
