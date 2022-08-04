---
layout: method
title: stlab::sender::close
tags: [library]
scope: sender
pure-name: close
defined-in-header: stlab/concurrency/channel.hpp 
declaration: close()
description: Close this side of the channel. No more values will be accepted. Already sent value will be processed
entities:
  - kind: methods
    list:
      - name: stlab::sender::close
        pure-name: close
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: void close()
        description: Closes this side of the channel, so it is not possible to send new values into it. But it does not destructs the channel.
---
