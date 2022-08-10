---
layout: method
title: channel_error
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/channel.hpp
is_ctor: true
overloads:
  channel_error(const stlab::channel_error &):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const stlab::channel_error &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: channel_error(const stlab::channel_error &)
  channel_error(stlab::channel_error &&):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: stlab::channel_error &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: channel_error(stlab::channel_error &&)
  explicit channel_error(stlab::channel_error_codes):
    arguments:
      - description: __OPTIONAL__
        name: code
        type: stlab::channel_error_codes
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: explicit channel_error(stlab::channel_error_codes code)
---
