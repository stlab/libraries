---
layout: library
title: Concurrency
tags: [library]
library-type: library
brief: This library provides high level abstractions for implementing algorithms that eases the use of multiple CPU cores while minimizing the contention.
description: |    
    The future implementation differs in several aspects compared to the C++11/14/17 standard futures: 
    It provides continuations and joins, which were just added in a C++17 TS. But more important this futures propagate values through the graph and not futures. This allows an easy way of creating splits. That means a single future can have multiple continuations into different directions. 
    
    An other important difference is that the futures support cancellation. So if one is not anymore interested in the result of a future, then one can destroy the future without the need to wait until the future is fulfilled, as it is the case with `std::future` (and `boost::future`). An already started future will run until its end, but will not trigger any continuation. So in all these cases, all chained continuations will never be triggered. Additionally the future interface is designed in a way, that one can use build in or custom build executors. 
    This implies too, that the user has to take more care, that all references used inside the task associated with the future must either exist, or their meanwhile absence must be taken into account. (One appropriate mechanism here are weak and shared pointer.)

    Since one can create with futures only graphs for single use, this library provides as well channels. With these channels one can build graphs, that can be used for multiple invocations.
---

### Requirements

* C++17 compliant compiler (clang, gcc 7, Visual Studio 2017)
* boost 1.60.0 (optional, variant, if they are not available from the compiler and test for executing the UnitTests)

### Authors
Sean Parent, Foster Brereton, Felix Petriconi and other contributors.

{% include example_table.html %}
