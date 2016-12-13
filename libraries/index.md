---
layout: library
title: libraries
entities:
  - kind: class
    name: stlab::future
    pure-name: future
    defined-in-header: stlab/future.hpp
    declaration: |
        template <typename T> class future;
    description: This class implements a future ibrary
    link: stlab_future
  - kind: class
    name: stlab::sender
    pure-name: sender
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> class sender;
    description: This class implements the sending part of a CSP channel
    link: stlab_sender
  - kind: class
    name: stlab::receiver
    pure-name: receiver
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> class receiver;
    description: This class implements the receiving part of a CSP channel
    link: stlab_receiver
  - kind: class
---
