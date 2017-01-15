---
layout: method
title: stlab::receiver
tags: [library]
full-name: stlab::receiver::operator|
pure-name: operator|
defined-in-header: stlab/channel.hpp 
declaration: operator|()
description: Attaches the given process to this receiver
entities:
  - kind: methods
    list:
      - name: stlab::receiver::operator|
        pure-name: operator|
        defined-in-header: stlab/channel.hpp 
        declaration: template <typename F> auto operator|(F&& f)
        description: Attaches the given process to the receiver and returns a new receiver. Itcan either be a functor with a single parameter or a process that follows this signatur.
  - kind: example
    code: NoCode
---
