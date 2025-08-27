---
layout: function
title: zip_with
hyde:
  owner: sean-parent
  brief: Creates a channel that zips multiple channels into a single one
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/concurrency/channel.hpp
  overloads:
    "template <typename S, typename F, typename... R>\nauto zip_with(S, F, const R &...)":
      arguments:
        - description: Executor which shall be used to combine the upstream values.
          name: s
          type: S
        - description: The attached process that is invoked with a new set of values.
          name: f
          type: F
        - description: The upstream receiver.
          name: upstream_receiver
          type: const R &...
      description: Creates a new receiver. The values coming from the upstream receiver collected and when from each upstream receiver a values is available, then it passes them to the process `f`.
      inline:
        description:
          - "***********************************************************************************************"
      return: a `receiver<T>` where `T is the result type of `f`
      signature_with_names: "template <typename S, typename F, typename... R>\nauto zip_with(S s, F f, const R &... upstream_receiver)"
  namespace:
    - stlab
    - v2_1_0
---
