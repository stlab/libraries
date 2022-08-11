---
layout: method
title: close
owner: FelixPetriconi
brief: Close this side of the channel.
tags:
  - method
defined_in_file: concurrency/channel.hpp
overloads:
  void close():
    description: Closes this side of the channel, so it is not possible to send new values into it. It does not destruct the channel; already sent values will be processed.
    return: __OPTIONAL__
    signature_with_names: void close()
---
