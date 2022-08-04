---
layout: post
title:  New Concurrency Library
categories: concurrency
comments: true
tags: concurrency library
---

A new concurrency library was added as a first one to this new stlab instance.

This library provides an alternative to the C++11/14 and upcoming C++17 TS futures. As well it provides an implementation of channels that allow the creation of process graphs.

<!--more-->

### Futures 
The future implementation differs in several aspects compared to the C++11/14/17 standard futures: It provides continuations and joins, which were just added in a C++17 TS. But more important this futures propagate values through the graph and not futures. This allows an easy way of creating splits. That means a single future can have multiple continuations into different directions. An other important difference is that the futures support cancellation. So if one is not anymore interested in the result of a future, then one can destroy the future without the need to wait that the future is fulfilled, as it is the case with std::future (and boost::future). An already started future will run until its end, but will not trigger any continuation. So in all these cases, all chained continuations will never be triggered. Additionally the future interface is designed in a way, that one can use build in or custom build executors. 

### Channels
Since one can create with futures only graphs for single use, this library provides as well channels. With these channels one can build graphs, that can be used for multiple invocations.

### Documentation
The full documentation can be found by following the libraries link on the top.
