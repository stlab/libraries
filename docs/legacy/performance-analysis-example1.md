---
title: Performance/Analysis/Example1
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Performance/Analysis/Example1
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Loop Unrolling and GCC

Let's take a look at the output from loop_unroll.cpp, compiled with <b>gcc</b> 4.0.1.
(not to pick on <b>gcc</b>, but it gives some very interesting results)

And to be specific:
<pre>
Apple PowerMac G5 2.5 Ghz Quad Processor
gcc version 4.0.1 (Apple Computer, Inc. build 5367)
</pre>

<pre>g++ -I. -O3 loop_unroll.cpp -o loop_unroll_gcc
./loop_unroll_gcc > "loop_unroll gccO3.txt"</pre>

<hr>
### Integer loop unrolling
I'm only copying the first 10 results of each section to save space.

<pre>
test                description   absolute   operations   ratio with
number                            time       per second   test0

 0  "int32_t for loop unroll 1"  12.10 sec   198.35 M     1.00
 1  "int32_t for loop unroll 2"  13.94 sec   172.17 M     1.15
 2  "int32_t for loop unroll 3"  10.61 sec   226.20 M     0.88
 3  "int32_t for loop unroll 4"   8.91 sec   269.36 M     0.74
 4  "int32_t for loop unroll 5"   9.62 sec   249.48 M     0.80
 5  "int32_t for loop unroll 6"   9.77 sec   245.65 M     0.81
 6  "int32_t for loop unroll 7"   9.16 sec   262.01 M     0.76
 7  "int32_t for loop unroll 8"   9.86 sec   243.41 M     0.81
 8  "int32_t for loop unroll 9"   9.31 sec   257.79 M     0.77
 9 "int32_t for loop unroll 10"   8.96 sec   267.86 M     0.74</pre>

The times are more or less decreasing with the unroll factor, and the minimum time is reached after an unroll factor of 4 or more.
This is close to the pattern we expect to see when loops are unrolled by hand and the compiler doesn't do any further unrolling.
In fact, with only -O3, <b>gcc</b> does not unroll loops (missing out on an important optimization!).
So later, we'll have to try another test with <b>gcc</b>'s -funroll-loops flag.

Because unrolling by a factor of 2 got slower than a non-unrolled loop, we can also guess that <b>gcc</b>
has some trouble scheduling the instructions (it should have been the same or faster).
A look at the assembly code confirms that the factor of 2 case has a few more stalls than the non-unrolled case.
I am a little worried that the times go back up when unrolling by a factor of 5 or more.
But another look at the assembly shows that it's just more less-than-optimal scheduling.

Alright, on to the <b>while</b> loops.  These look about the same as the <b>for</b> loops.
<pre>
test                  description   absolute   operations   ratio with
number                              time       per second   test0

 0  "int32_t while loop unroll 1"  12.11 sec   198.18 M     1.00
 1  "int32_t while loop unroll 2"  13.93 sec   172.29 M     1.15
 2  "int32_t while loop unroll 3"  10.60 sec   226.42 M     0.88
 3  "int32_t while loop unroll 4"   8.90 sec   269.66 M     0.73
 4  "int32_t while loop unroll 5"   9.61 sec   249.74 M     0.79
 5  "int32_t while loop unroll 6"   9.78 sec   245.40 M     0.81
 6  "int32_t while loop unroll 7"   9.19 sec   261.15 M     0.76
 7  "int32_t while loop unroll 8"   9.86 sec   243.41 M     0.81
 8  "int32_t while loop unroll 9"   9.32 sec   257.51 M     0.77
 9 "int32_t while loop unroll 10"   8.95 sec   268.16 M     0.74</pre>

The <b>do</b> loops are just a little different.
<pre>
test               description   absolute   operations   ratio with
number                           time       per second   test0

 0  "int32_t do loop unroll 1"  11.22 sec   213.90 M     1.00
 1  "int32_t do loop unroll 2"  13.46 sec   178.31 M     1.20
 2  "int32_t do loop unroll 3"  10.00 sec   240.00 M     0.89
 3  "int32_t do loop unroll 4"   8.89 sec   269.97 M     0.79
 4  "int32_t do loop unroll 5"   9.24 sec   259.74 M     0.82
 5  "int32_t do loop unroll 6"   9.78 sec   245.40 M     0.87
 6  "int32_t do loop unroll 7"   8.55 sec   280.70 M     0.76
 7  "int32_t do loop unroll 8"   9.74 sec   246.41 M     0.87
 8  "int32_t do loop unroll 9"   9.30 sec   258.06 M     0.83
 9 "int32_t do loop unroll 10"   8.67 sec   276.82 M     0.77</pre>

<b>do</b> loops are faster <b>for</b> the non-unrolled case than <b>for</b> or <b>while</b> loops.
That means that <b>gcc</b> may have some problems normalizing the different loop forms or with
instantiating the templates.  Unfortunately, reading the assembly and compiler intermediate data
didn't help me identify the exact cause.
But it is still close to the results for the <b>for</b> loops after unrolling by a factor of 2 or more.

Finally, the <b>goto</b> loops look similar to the <b>do</b> loops.
<pre>
test                 description   absolute   operations   ratio with
number                             time       per second   test0

 0  "int32_t goto loop unroll 1"  11.21 sec   214.09 M     1.00
 1  "int32_t goto loop unroll 2"  13.46 sec   178.31 M     1.20
 2  "int32_t goto loop unroll 3"  10.18 sec   235.76 M     0.91
 3  "int32_t goto loop unroll 4"   8.90 sec   269.66 M     0.79
 4  "int32_t goto loop unroll 5"   9.24 sec   259.74 M     0.82
 5  "int32_t goto loop unroll 6"   9.77 sec   245.65 M     0.87
 6  "int32_t goto loop unroll 7"   8.56 sec   280.37 M     0.76
 7  "int32_t goto loop unroll 8"   9.74 sec   246.41 M     0.87
 8  "int32_t goto loop unroll 9"   9.30 sec   258.06 M     0.83
 9 "int32_t goto loop unroll 10"   8.66 sec   277.14 M     0.77</pre>


#### Conclusions for int32_t loop unrolling under <b>gcc</b>:
<ul>
<li>With the -O3 flag for optimization, <b>gcc</b> is not unrolling loops.</li>
<li><b>gcc</b> can generate faster code for many of these loops if it unrolled them.</li>
<li><b>gcc</b> can generate faster code for the non-unrolled <b>for</b> and <b>while</b> loops if
it generated code similar to the <b>do</b> and <b>goto</b> loops.</li>
<li><b>gcc</b> has some instruction scheduling problems on my CPU.</li>
</ul>

<hr>
### Double loop unrolling

Now let's take a look at the double precision loop unrolling results.

<pre>test               description   absolute   operations   ratio with
number                           time       per second   test0

 0  "double for loop unroll 1"   3.13 sec   191.69 M     1.00
 1  "double for loop unroll 2"   2.23 sec   269.06 M     0.71
 2  "double for loop unroll 3"   3.91 sec   153.45 M     1.25
 3  "double for loop unroll 4"   7.46 sec    80.43 M     2.38
 4  "double for loop unroll 5"   6.90 sec    86.96 M     2.20
 5  "double for loop unroll 6"   5.79 sec   103.63 M     1.85
 6  "double for loop unroll 7"   6.30 sec    95.24 M     2.01
 7  "double for loop unroll 8"   6.65 sec    90.23 M     2.12
 8  "double for loop unroll 9"   7.07 sec    84.87 M     2.26
 9 "double for loop unroll 10"   7.30 sec    82.19 M     2.33</pre>

The times go down, then up, way up, down a little, up again, and... What the heck?
If a compiler doesn't unroll the loops beyond what the template did,
the times should decrease with larger unrolling factors.
And we know that with -O3, <b>gcc</b> is not unrolling the loops.

The <b>while</b> loops look about the same as the <b>for</b> loops.
The <b>do</b> and <b>goto</b> loops are again a little faster for the non-unrolled case.
But all of the double loops show the same unexpected timings.

What is going on?

I tried to isolate a few of the double tests so I could look at the assembly, but when I ran it I got:
<pre>
test               description   absolute   operations   ratio with
number                           time       per second   test0

 0  "double for loop unroll 1"   3.13 sec   191.69 M     1.00
 1  "double for loop unroll 2"   2.23 sec   269.06 M     0.71
 2  "double for loop unroll 3"   1.99 sec   301.51 M     0.64
 3  "double for loop unroll 4"   1.95 sec   307.69 M     0.62
 4  "double for loop unroll 5"   1.92 sec   312.50 M     0.61
 5  "double for loop unroll 6"   1.82 sec   329.67 M     0.58
 6  "double for loop unroll 7"   1.95 sec   307.69 M     0.62
 7  "double for loop unroll 8"   2.01 sec   298.51 M     0.64
 8  "double for loop unroll 9"   2.06 sec   291.26 M     0.66
 9 "double for loop unroll 10"   2.02 sec   297.03 M     0.65</pre>

Wait! That's not the same!

When I add back some of the other tests (<b>while</b>, <b>do</b>, <b>goto</b>, or the integer tests)
the times get worse - but not in any predictable pattern that I could see.
Somehow, <b>gcc</b> is having problems with these loops because of the surrounding code.

Sadly, the results are very similar if I compile with -funroll-loops.

#### Conclusions for double loop unrolling under <b>gcc</b>:
<ul><li>I can't draw any conclusions about unrolling loops with double precision math using this code,
until some bugs are fixed in <b>gcc</b>.</li>
<li><i>[I confirmed with hand unrolled code that the double loops show the same timing
patterns as the integer loops. The <b>gcc</b> bug seems related to template instantiation -- ccox]</i></li>
</ul>

<hr>
### Integer loops with -funroll-loops

Now let's see what happens when we explicitly tell <b>gcc</b> to unroll the loops.
Of course, adding the extra flag is not allowed by the benchmark rules, but it is interesting to
know what would happen if <b>gcc</b> did turn on loop unrolling.

<pre>g++ -I. -O3 -funroll-loops loop_unroll.cpp -o loop_unroll_gcc_unrolled
./loop_unroll_gcc_unrolled > "loop_unroll gccO3unroll.txt"</pre>

<pre>
test                description   absolute   operations   ratio with
number                            time       per second   test0

 0  "int32_t for loop unroll 1"  10.30 sec   233.01 M     1.00
 1  "int32_t for loop unroll 2"  10.57 sec   227.06 M     1.03
 2  "int32_t for loop unroll 3"  10.62 sec   225.99 M     1.03
 3  "int32_t for loop unroll 4"   8.90 sec   269.66 M     0.86
 4  "int32_t for loop unroll 5"   9.62 sec   249.48 M     0.93
 5  "int32_t for loop unroll 6"   9.78 sec   245.40 M     0.95
 6  "int32_t for loop unroll 7"   9.22 sec   260.30 M     0.90
 7  "int32_t for loop unroll 8"   9.86 sec   243.41 M     0.96
 8  "int32_t for loop unroll 9"   9.32 sec   257.51 M     0.90
 9 "int32_t for loop unroll 10"   8.95 sec   268.16 M     0.87</pre>

The non-unrolled case, and the unrolled by 2 cases have improved a little.
The rest of the cases look about the same as without -funroll-loops.
It looks like <b>gcc</b> is unrolling small loops, but not larger loops.
And <b>gcc</b> is not unrolling them enough to get optimal performance, or has trouble scheduling
the instructions.  A look at the assembly shows that <b>gcc</b> unrolled the non-unrolled case by a
factor of 4 (good!), but scheduled the instructions rather poorly.
The unroll factor 2 case was unrolled by an additional factor of 2, and scheduled poorly.
Ok, so <b>gcc</b> is trying to unroll the loops and just not scheduling the instructions very well after unrolling.

The <b>while</b> loops look very similar to the <b>for</b> loop results.
<pre>
test                  description   absolute   operations   ratio with
number                              time       per second   test0

 0  "int32_t while loop unroll 1"  10.27 sec   233.69 M     1.00
 1  "int32_t while loop unroll 2"  10.58 sec   226.84 M     1.03
 2  "int32_t while loop unroll 3"  10.61 sec   226.20 M     1.03
 3  "int32_t while loop unroll 4"   8.90 sec   269.66 M     0.87
 4  "int32_t while loop unroll 5"   9.62 sec   249.48 M     0.94
 5  "int32_t while loop unroll 6"   9.78 sec   245.40 M     0.95
 6  "int32_t while loop unroll 7"   9.23 sec   260.02 M     0.90
 7  "int32_t while loop unroll 8"   9.87 sec   243.16 M     0.96
 8  "int32_t while loop unroll 9"   9.31 sec   257.79 M     0.91
 9 "int32_t while loop unroll 10"   8.97 sec   267.56 M     0.87</pre>

The <b>do</b> loops show the same oddity on the non-unrolled case as the integer loops.
<pre>
test               description   absolute   operations   ratio with
number                           time       per second   test0

 0  "int32_t do loop unroll 1"   8.42 sec   285.04 M     1.00
 1  "int32_t do loop unroll 2"  10.70 sec   224.30 M     1.27
 2  "int32_t do loop unroll 3"  10.09 sec   237.86 M     1.20
 3  "int32_t do loop unroll 4"   9.86 sec   243.41 M     1.17
 4  "int32_t do loop unroll 5"   9.23 sec   260.02 M     1.10
 5  "int32_t do loop unroll 6"   9.78 sec   245.40 M     1.16
 6  "int32_t do loop unroll 7"   8.57 sec   280.05 M     1.02
 7  "int32_t do loop unroll 8"   9.75 sec   246.15 M     1.16
 8  "int32_t do loop unroll 9"   9.31 sec   257.79 M     1.11
 9 "int32_t do loop unroll 10"   8.68 sec   276.50 M     1.03</pre>

And the <b>goto</b> loops look about the same as the <b>do</b> loops.
<pre>
test                 description   absolute   operations   ratio with
number                             time       per second   test0

 0  "int32_t goto loop unroll 1"   8.42 sec   285.04 M     1.00
 1  "int32_t goto loop unroll 2"  10.69 sec   224.51 M     1.27
 2  "int32_t goto loop unroll 3"  10.16 sec   236.22 M     1.21
 3  "int32_t goto loop unroll 4"   9.84 sec   243.90 M     1.17
 4  "int32_t goto loop unroll 5"   9.24 sec   259.74 M     1.10
 5  "int32_t goto loop unroll 6"   9.77 sec   245.65 M     1.16
 6  "int32_t goto loop unroll 7"   8.56 sec   280.37 M     1.02
 7  "int32_t goto loop unroll 8"   9.73 sec   246.66 M     1.16
 8  "int32_t goto loop unroll 9"   9.29 sec   258.34 M     1.10
 9 "int32_t goto loop unroll 10"   8.67 sec   276.82 M     1.03</pre>

#### Conclusions for int32_t loop unrolling, with -funroll-loop:
<ul><li>With the -funroll-loops flag, <b>gcc</b> is unrolling some smaller loops.</li>
<li><b>gcc</b> can generate faster code for these loops if it did a better job of scheduling
the instructions after unrolling.</li>
<li><b>gcc</b> can still generate faster code for the non-unrolled <b>for</b> and <b>while</b>
loops if it generated code similar to the <b>do</b> and <b>goto</b> loops.</li>
</ul>
