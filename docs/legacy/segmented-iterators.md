---
title: Segmented Iterators
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Segmented_Iterators
---

A segmented iterator (also sometimes referred to as a nested iterator, multi-dimentional iterator, etc.) is an iterator which iterates over a collection of ranges (segments).

For example, in STL, a deque iterator is a segmented iterator (a deque is implemented as chunks).

Many algorithms which operate on segmented iterator could be implemented more efficiently if they had access to the segments.

The concept of a segmented iterator is closely tied to bounded algorithms, as bounded algorithms are a building block for many efficient segmented algorithms. Segmented iterators seem to pop out often in dealing with N-dimentional structures (like a matrix), blocked structures (such as a deque), they appear in the circular_queue in this library, they appear in many tree structures... as such I'm convinced there is a valid concept underlying segmented iterators which is worth exploring.

One difficulty in dealing with segmented iterators is the concept is orthogonal to the current iterator categories and the mechanisms for dealing with orthogonal concepts is not well defined.

Here I'm going to start with a loose Concept for a SegmentedSequence and a SegmentedIterator. Then implement some rudimentary algorithms on segmented iterators (partially this will be done to support circular_queue and a request to be able to use circular_queue for efficiently calculating moving averages - this will also however, greatly simplify the implementation of operator==() in the current circular_queue.
