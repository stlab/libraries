---
layout: method
title: stlab::sender
tags: [library]
full-name: stlab::sender::close
pure-name: close
defined-in-header: stlab/channel.hpp 
declaration: close()
description: Constructs this side of the channel
entities:
  - kind: methods
    list:
      - name: stlab::sender::close
        pure-name: close
        defined-in-header: stlab/channel.hpp 
        declaration: void close()
        description: Closes this side of the channel, so it is not possible to send new values into it. But it does not destructs the channel.
  - kind: example
    code: NoCode
---
