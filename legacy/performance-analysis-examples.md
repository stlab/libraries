---
title: Performance/Analysis/Examples
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Performance/Analysis/Examples
---

## What do all those numbers mean?

Ok, you've downloaded the benchmark source, compiled and run it, and now you have a text file with a lot of numbers.
How do you interpret those numbers?

You are looking at the raw numbers from each test, not a weighted average or formatted summary.
Eventually, we will have a web site where you can look at the raw numbers from different compilers and platforms,
as well as analysis that has broken down the raw numbers into something a little smaller and easier to digest.
But that's going to take time to build (both the analysis and the web site).
For now, these are engineering numbers, not marketing numbers. You can think of it as a list of
times taken by each of the runners in a track event, not the short list of olympic medal winners -- useful if you
want to do some analysis, but not as useful if all you want to know is the winner.

## Examples of analyzing benchmark results:

[loop_unroll.cpp with gcc](performance-analysis-example1.html)

[simple_types_loop_invariant.cpp with LLVM](performance-analysis-example2.html)

[stepanov_abstraction.cpp with MSVC](performance-analysis-example3.html)
