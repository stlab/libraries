---
layout: method
title: ready
owner: FelixPetriconi
brief: Returns `true` if the receiver is ready, otherwise `false`.
tags:
  - method
defined_in_file: concurrency/channel.hpp
overloads:
  bool ready() const:
    description: If the receiver was set to ready, it returns `true`, otherwise `false`; the initial value is `false`.
    return: "`true` if this receiver is ready to receive values."
    signature_with_names: bool ready() const
---
