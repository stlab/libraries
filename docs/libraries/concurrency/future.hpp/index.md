---
layout: library
title: concurrency/future.hpp
owner: sean-parent
brief: Futures class and related functions
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  context_result_t:
    definition: std::conditional_t<std::is_same<void, param_t>::value, void, param_t>
    description: __MISSING__
  context_t:
    definition: detail::common_context<detail::context_result<F, true, context_result_t>, F, detail::single_trigger, detail::all_trigger>
    description: __MISSING__
  opt_t:
    definition: optional_placeholder_tuple<Ts...>
    description: __MISSING__
  param_t:
    definition: typename std::iterator_traits<I>::value_type::result_type
    description: __MISSING__
  result_t:
    definition: typename detail::result_of_when_any_t<F, param_t>::result_type
    description: __MISSING__
  result_type:
    definition: detail::result_t<std::decay_t<F>, std::decay_t<Args>...>
    description: __MISSING__
  vt_t:
    definition: voidless_tuple<Ts...>
    description: __MISSING__
---
