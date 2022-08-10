---
layout: function
title: get
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/variant.hpp
overloads:
  "template <class T, class... Types>\nconstexpr T & get(std::variant<Types...> &)":
    arguments:
      - description: __OPTIONAL__
        name: v
        type: std::variant<Types...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class... Types>\nconstexpr T & get(std::variant<Types...> & v)"
  "template <class T, class... Types>\nconstexpr T && get(std::variant<Types...> &&)":
    arguments:
      - description: __OPTIONAL__
        name: v
        type: std::variant<Types...> &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class... Types>\nconstexpr T && get(std::variant<Types...> && v)"
  "template <class T, class... Types>\nconstexpr const T & get(const std::variant<Types...> &)":
    arguments:
      - description: __OPTIONAL__
        name: v
        type: const std::variant<Types...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class... Types>\nconstexpr const T & get(const std::variant<Types...> & v)"
  "template <class T, class... Types>\nconstexpr const T && get(const std::variant<Types...> &&)":
    arguments:
      - description: __OPTIONAL__
        name: v
        type: const std::variant<Types...> &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class... Types>\nconstexpr const T && get(const std::variant<Types...> && v)"
namespace:
  - stlab
---
