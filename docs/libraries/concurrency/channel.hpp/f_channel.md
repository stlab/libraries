---
layout: function
title: channel
owner: sean-parent
brief: Creates a pair that consists of a sender and a receiver
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  "template <typename T, typename E>\nauto channel(E)":
    arguments:
      - description: Executor which is used to send the passed values from the sender down to the receiver.
        name: executor
        type: E
    description: Creates a channel that consists of a sending and a receiving part of the channel.
    return: A pair of `sender` - `receiver` of type `T` that form a channel in case that `T` is not of type `void`. In case of type `void` it returns only a channel of type `void`.
    signature_with_names: "template <typename T, typename E>\nauto channel(E executor)"
namespace:
  - stlab
  - v1
---
