---
layout: library
title: stlab/enum_ops.hpp
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - sourcefile
  library-type: sourcefile
  typedefs:
    enable_if_bitmask_or_arithmetic:
      definition: std::enable_if_t<std::disjunction_v<stlab::implementation::has_enabled_bitmask_t<T>, stlab::implementation::has_enabled_arithmetic_t<T>>, U>
      description: __MISSING__
    has_enabled_arithmetic_t:
      definition: decltype(stlab_enable_arithmetic_enum(std::declval<T>()))
      description: __MISSING__
    has_enabled_bitmask_t:
      definition: decltype(stlab_enable_bitmask_enum(std::declval<T>()))
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
    is_convertible_to_underlying:
      definition: std::is_convertible<U, stlab::implementation::safe_underlying_type_t<T>>
      description: __MISSING__
    safe_underlying_type_t:
      definition: typename safe_underlying_type<T, std::is_enum<T>::value>::type
      description: __MISSING__
    underlying:
      definition: std::underlying_type_t<T>
      description: __MISSING__
---
