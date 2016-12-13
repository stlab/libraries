---
layout: function
title: stlab::when_all
entities:
  - name: stlab::when_all
    pure-name: when_all
    defined-in-header: stlab/future.hpp
    declaration: |
        template <typename S, typename F, typename...Ts>
        auto when_all(S s, F f, future<Ts>... args);
    description: This function create a joining future 
    result: TBD
  - name: stlab::when_all
    pure-name: when_all
    defined-in-header: stlab/future.hpp
    declaration: |
        template <typename S, typename F, typename I> 
        auto when_all(S s, F f, const std::pair<I,I>& range);
    description: This function create a joining future 
    result: TBD

---
