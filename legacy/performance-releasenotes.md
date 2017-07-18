---
title: Performance/ReleaseNotes
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Performance/ReleaseNotes
---
#### Contents
{:.no_toc}
1. contents
{:toc}

# Release 2 - December 8, 2008

## Known Problems
<ul>

<li>Line out of place in functionobjects.cpp near line 334.<br>
<i><b>Moved the line to the correct location.</b></i>
</li>

<li>makefile.nt was not updated to include the new test files<br>
<i><b>Fixed makefile.nt, posted update.</b></i>
</li>

</ul>
<br>

## Files

<dl>

<dt>functionobjects.cpp</dt>
<dd>
This is a demonstration of the performance of of function pointers, functors, and native comparison operators.  Some compilers have difficulty instantiating simple functors.
</dd>
<dt>simple_types_constant_folding.cpp</dt>
<dd>
A test to see if the compiler will correctly fold constants and simple constant math for simple types.  Results may be surprising.
</dd>
<dt>stepanov_vector.cpp</dt>
<dd>
Similar to the abstraction penalty benchmark, this file tests what happens when we move from pointers to vector iterators and when we use reverse iterators.  This tests the compiler supplied STL implementation in addition to the compiler itself.  Some good compilers are crippled by bad STL implementations.
</dd>





</dl>



# Release 1 - May 5, 2008

## Fixed Problems
<ul>
<li>Some Linux distributions have incomplete MACH/gcc headers.  I need a simple way to determine whether they have the TR1 headers or not.  Unfortunately, I don't have any of those distributions.
<i><b>Related code is now commented out.</b></i>
</li>

<li>Some 64 bit Linux distributions don't like benchmark_stdint.hpp.
<i><b>Fix is posted, but really needs to be tested on more OSes.</b></i>
</li>

<li>At least one embedded compiler didn't like the order of templates in loop_unroll.cpp.
<i><b>Changed the template order.</b></i>
</li>

</ul>
<br>

<h2>Files</h2>

<dl>

<dt>loop_unroll.cpp</dt>
<dd>
Test to see if compilers will correctly unroll loops to hide instruction latency.  Some compilers have problems expanding the templates, and most compilers have problems correctly unrolling the loops for best performance.
</dd>
<dt>simple_types_loop_invariant.cpp</dt>
<dd>
A test to see if the compiler will move loop invariant calculations out of the loop.  Most compilers have room for improvement.
</dd>
<dt>stepanov_abstraction.cpp</dt>
<dd>
An expanded version of the original test, answering "what happens to performance when I wrap a value in curly braces?"  Almost all compilers do well on the original summation tests, but they don't do nearly so well on simple sort routines using the same abstractions.
</dd>



<dt>machine.cpp</dt>
<dd>
A utility program to print out information about the compiler version, OS, and machine environment - because it is nice to know which build of your compiler generated a particular report, and which of the 30 machines in your lab that it was run on.
</dd>



<dt>benchmark_algorithms.h</dt>
<dd>
Templated algorithms shared by several test files.
</dd>
<dt>benchmark_results.h</dt>
<dd>
Shared result reporting and formatting functions for all tests, must work in C and C++.
</dd>
<dt>benchmark_shared_tests.h</dt>
<dd>
Templated test functions shared by several test files.
</dd>
<dt>benchmark_stdint.h</dt>
<dd>
Standard type definitions, required because some compilers still have not picked up C99 standard headers.
</dd>
<dt>benchmark_timer.h</dt>
<dd>
Shared timer function for all tests, must work in C and C++.
</dd>


<dt>makefile</dt>
<dd>
Make information for Unix based OSes.  "make report" to build all binaries and start a benchmark run.
</dd>
<dt>makefile.nt</dt>
<dd>
Make information for Windows.  "nmake -f makefile.nt report" (in a shell/command prompt with the appropriate compiler variables set) to build all binaries and start a benchmark run.
</dd>


<dt>LICENSE_1_0_0.txt</dt>
<dd>
License details, a copy can also be found at <a href='https://stlab.adobe.com/licenses.html'>https://stlab.adobe.com/licenses.html</a>.
</dd>
<dt>README.txt</dt>
<dd>
Basic information about the benchmark suite: goals, rules, and instructions on how to build and run.
</dd>


</dl>
