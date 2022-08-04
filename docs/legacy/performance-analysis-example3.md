---
title: Performance/Analysis/Example3
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Performance/Analysis/Example3
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Stepanov Abstraction with MSVC

Now we'll examine the results from stepanov_abstraction.cpp compiled with Microsoft Visual C++ (<b>MSVC</b>) 2008 and 2005.


And to be specific:
<pre>
Intel Core2 QuadCore 2.66Ghz
Windows Vista 64 bit Ultimate

MSVC 2008:
Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 15.00.21022.08 for 80x86

MSVC 2005:
Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 14.00.50727.762 for 80x86
</pre>

<pre>
cl /nologo /Ox /TP /EHsc /D "WIN32" /D "NDEBUG" /Wp64 stepanov_abstraction.cpp -o stepanov_abstraction.exe
.\stepanov_abstraction > stepanov_abstraction_msvc2008.txt
</pre>


<hr>
## MSVC 2008

### Accumulate
<pre>
test                        description   absolute   operations   ratio with
number                                    time       per second   test0

 0                     "double pointer"   4.60 sec   869.19 M     1.00
 1               "double pointer_class"   4.59 sec   872.22 M     1.00
 2         "DoubleValueWrapper pointer"   4.54 sec   881.06 M     0.99
 3   "DoubleValueWrapper pointer_class"   4.55 sec   878.16 M     0.99
 4       "DoubleValueWrapper10 pointer"   4.57 sec   875.08 M     0.99
 5 "DoubleValueWrapper10 pointer_class"   4.54 sec   881.25 M     0.99
</pre>

This is good. The compiler had no significant penalty for increasing the data or pointer abstractions in the accumulation loops.


### Insertion Sort
<pre>
test                                       description   absolute   operations   ratio with
number                                                   time       per second   test0

 0                     "insertion_sort double pointer"   0.86 sec    2.33 M     1.00
 1               "insertion_sort double pointer_class"   1.19 sec    1.69 M     1.38
 2         "insertion_sort DoubleValueWrapper pointer"   1.19 sec    1.69 M     1.38
 3   "insertion_sort DoubleValueWrapper pointer_class"   1.56 sec    1.28 M     1.82
 4       "insertion_sort DoubleValueWrapper10 pointer"   1.17 sec    1.71 M     1.36
 5 "insertion_sort DoubleValueWrapper10 pointer_class"   1.56 sec    1.28 M     1.82
</pre>

This isn't so good -- all of the times should have been the same and the ratios close to 1.0.
Increasing the abstraction on the value and the pointer both appear to cause slowdowns.


### Quicksort
<pre>
test                                  description   absolute   operations   ratio with
number                                              time       per second   test0

 0                     "quicksort double pointer"   1.22 sec   13.15 M     1.00
 1               "quicksort double pointer_class"   1.34 sec   11.92 M     1.10
 2         "quicksort DoubleValueWrapper pointer"   1.51 sec   10.58 M     1.24
 3   "quicksort DoubleValueWrapper pointer_class"   1.61 sec    9.96 M     1.32
 4       "quicksort DoubleValueWrapper10 pointer"   1.51 sec   10.58 M     1.24
 5 "quicksort DoubleValueWrapper10 pointer_class"   1.61 sec    9.96 M     1.32
</pre>

Again, we have some slowdowns when increasing abstraction, but not as much as we saw in the insertion sort.
That is partly due to the different complexities of the algorithms being tested:  Insertion sort is O(N^2) and Quicksort is O(NLogN).  Also, the 3 sort algorithms in this test file use pointers, values, indexing and incrementing very differently.

### Heap Sort
<pre>
test                                  description   absolute   operations   ratio with
number                                              time       per second   test0

 0                     "heap_sort double pointer"   1.36 sec   11.79 M     1.00
 1               "heap_sort double pointer_class"   1.42 sec   11.27 M     1.05
 2         "heap_sort DoubleValueWrapper pointer"   1.37 sec   11.66 M     1.01
 3   "heap_sort DoubleValueWrapper pointer_class"   1.51 sec   10.57 M     1.12
 4       "heap_sort DoubleValueWrapper10 pointer"   1.39 sec   11.53 M     1.02
 5 "heap_sort DoubleValueWrapper10 pointer_class"   1.72 sec    9.32 M     1.26
</pre>

And we still have slowdowns with increasing abstraction.


#### Conclusions for MSVC 2008:
<ul>
<li><b>MSVC 2008</b> could improve code generation related to C++ abstraction.</li>
</ul>



<hr>
## MSVC 2005 Comparison

### Accumulate MSVC2005
<pre>
test                        description   absolute   operations   ratio with
number                                    time       per second   test0

 0                     "double pointer"   4.55 sec   878.16 M     1.00
 1               "double pointer_class"   4.55 sec   878.16 M     1.00
 2         "DoubleValueWrapper pointer"   4.54 sec   881.06 M     1.00
 3   "DoubleValueWrapper pointer_class"   4.55 sec   878.16 M     1.00
 4       "DoubleValueWrapper10 pointer"   4.55 sec   878.16 M     1.00
 5 "DoubleValueWrapper10 pointer_class"   4.54 sec   881.06 M     1.00
</pre>

Good - very similar times, and no problems with increased abstraction.


### Insertion Sort MSVC2005
<pre>
test                                       description   absolute   operations   ratio with
number                                                   time       per second   test0

 0                     "insertion_sort double pointer"   1.19 sec    1.69 M     1.00
 1               "insertion_sort double pointer_class"   1.17 sec    1.71 M     0.99
 2         "insertion_sort DoubleValueWrapper pointer"   1.19 sec    1.69 M     1.00
 3   "insertion_sort DoubleValueWrapper pointer_class"   1.19 sec    1.69 M     1.00
 4       "insertion_sort DoubleValueWrapper10 pointer"   1.19 sec    1.69 M     1.00
 5 "insertion_sort DoubleValueWrapper10 pointer_class"   1.19 sec    1.69 M     1.00
</pre>

<b>MSVC 2008</b> produces faster code in one case, but significantly slower code in 2 cases.
And <b>MSVC 2005</b> doesn't have any penalty for increased abstraction -- all the times are close to the same.


### Quicksort MSVC2005
<pre>
test                                  description   absolute   operations   ratio with
number                                              time       per second   test0

 0                     "quicksort double pointer"   1.22 sec   13.16 M     1.00
 1               "quicksort double pointer_class"   1.23 sec   12.98 M     1.01
 2         "quicksort DoubleValueWrapper pointer"   1.54 sec   10.36 M     1.27
 3   "quicksort DoubleValueWrapper pointer_class"   1.54 sec   10.36 M     1.27
 4       "quicksort DoubleValueWrapper10 pointer"   1.56 sec   10.26 M     1.28
 5 "quicksort DoubleValueWrapper10 pointer_class"   1.54 sec   10.36 M     1.27
</pre>

This is close to the same as <b>MSVC 2008</b>, but faster in each case involving the pointer class.
There is still some penalty for using the value abstractions.


### Heap Sort MSVC2005
<pre>
test                                  description   absolute   operations   ratio with
number                                              time       per second   test0

 0                     "heap_sort double pointer"   1.36 sec   11.79 M     1.00
 1               "heap_sort double pointer_class"   1.37 sec   11.65 M     1.01
 2         "heap_sort DoubleValueWrapper pointer"   1.37 sec   11.65 M     1.01
 3   "heap_sort DoubleValueWrapper pointer_class"   1.37 sec   11.65 M     1.01
 4       "heap_sort DoubleValueWrapper10 pointer"   1.40 sec   11.40 M     1.03
 5 "heap_sort DoubleValueWrapper10 pointer_class"   1.37 sec   11.66 M     1.01
</pre>

This is much better than <b>MSVC 2008</b>, faster on 3 tests, and showing little penalty for increased abstraction.


#### Conclusions for MSVC 2008 compared with MSVC 2005:
<ul>
<li><b>MSVC 2008</b> produced faster code then <b>MSVC 2005</b> in one test.</li>
<li><b>MSVC 2008</b> produced slower code than <b>MSVC 2005</b> in 8 tests.</li>
<li><b>MSVC 2008</b> has increased abstraction penalties in the insertion sort and heap sort tests.</li>
<li><b>MSVC 2008</b> seems to have the most problems in code involving pointer_class.</li>
</ul>
