---
layout: function
title: stlab::when_any
entities:
  - name: stlab::when_any
    pure-name: when_any
    defined-in-header: stlab/future.hpp
    declaration: template <typename S, typename F, typename...Ts> auto when_any(S s, F f, future<Ts>... args);
    description: This function create a future that continues whenever the first future argument succeeds
    result: TBD
  - name: stlab::when_any
    pure-name: when_any
    defined-in-header: stlab/future.hpp
    declaration: template <typename S, typename F, typename I> auto when_any(S s, F f, const std::pair<I,I>& range);
    description: This function create a future that continues whenever the first future within the range succeeds
    result: TBD
---
