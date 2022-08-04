---
title: Performance/Analysis/Example2
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Performance/Analysis/Example2
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Loop Invariant Code Motion and LLVM

Now let's take a look at the results from simple_types_loop_invariant.cpp<P>

We'll use <b>LLVM-gcc</b> 2.1, specifically:
<pre>
gcc version 4.0.1 (Apple Computer, Inc. build 5449)(LLVM build 2.1)
Apple PowerMac G5 2.5 Ghz Quad Processor
</pre>

<pre>llvm-g++ -I. -O3 simple_types_loop_invariant.cpp -o simple_types_loop_invariant_llvm
./simple_types_loop_invariant_llvm > simple_types_loop_invariant_llvm.txt</pre>

<hr>
## Integer math
The int32_t case is fairly representative of all the integer results under <b>LLVM</b>.

<pre>
test                          description   absolute   operations   ratio with
number                                      time       per second   test0

 0                 "int32_t variable add"   2.57 sec   622.57 M     1.00
 1       "int32_t multiple variable adds"   2.57 sec   622.57 M     1.00
 2            "int32_t variable subtract"   2.57 sec   622.57 M     1.00
 3  "int32_t multiple variable subtracts"   3.52 sec   454.55 M     1.37
 4            "int32_t variable multiply"   3.84 sec   416.67 M     1.49
 5 "int32_t multiple variable multiplies"   3.86 sec   414.51 M     1.50
 6              "int32_t variable divide"  24.36 sec    65.68 M     9.48
 7    "int32_t multiple variable divides"  53.19 sec    30.08 M    20.70
 8      "int32_t multiple variable mixed"   3.86 sec   414.51 M     1.50
 9                 "int32_t variable and"   1.95 sec   820.51 M     0.76
10        "int32_t multiple variable and"   1.99 sec   804.02 M     0.77
11                  "int32_t variable or"   2.56 sec   625.00 M     1.00
12         "int32_t multiple variable or"   2.58 sec   620.16 M     1.00
13                 "int32_t variable xor"   1.99 sec   804.02 M     0.77
14        "int32_t multiple variable xor"   1.96 sec   816.33 M     0.76
</pre>

### addition
This is good.  Multiple loop invariant adds are the same speed as a single add.
And that means the compiler moved the loop invariant calculations out of the loop.

### subtraction
Hmm, multiple loop invariant subtracts take longer than a single subtract.
That means that <b>LLVM</b> failed to optimize loop invariant subtraction.
And that is really surprising because it did optimize addition.

<pre>
for (x=0;x&lt;count;++x)
	result += input[x] - v1 - v2 - v3 - v4;
</pre>

should have been optimized to
<pre>
temp = v1 + v2 + v3 + v4;
for (x=0;x&lt;count;++x)
	result += input[x] - temp;
</pre>

An assembly dump shows that <b>LLVM</b> issues each subtract inside the loop.

### multiplication
Also good.  This runs a little slower than adds due to instruction latency, but is optimized correctly.

### division
Clearly not optimized.  Hmm, what was the source for this?

<pre>
for (x=0;x&lt;count;++x)
	result += ((((input[x] / v1) / v2) / v3) / v4);
</pre>

Whoops! That can't be optimized for integer math without getting wrong results.
I messed up and used the wrong test code, and failed to catch it for months.
I'll have to change the test to include something that can be moved out of the loop.

### mixed math
This should be the same speed as a single add, but isn't. And yet it is not as slow as if all the operations were done in the inner loop.

<pre>
for (x=0;x&lt;count;++x)
	result += input[x] + v1 - v2 * v3 / v4;
</pre>

should have been optimized to
<pre>
temp = v1 - v2 * v3 / v4;
for (x=0;x&lt;count;++x)
	result += input[x] + temp;
</pre>

Reading the assembly shows that <b>LLVM</b> did move the multiply and divide out of the loop, but kept 2 adds and a subtract.
This is probably related to whatever caused the subtract case to fail earlier.

### bitwise and
Good.

### bitwise or
Both versions run about the same speed, but how is this running slower than a <b>bitwise and</b>?
Reading the assembly shows that <b>LLVM</b> did optimize the multiple <b>bitwise or</b>s correctly,
generating almost identical code to the <b>bitwise and</b> loops.
But there's an interesting problem - the 7 instruction <b>bitwise or</b> loops straddle a cacheline boundary.
On some CPUs (like the PowerPC 970 aka G5) that can add significant stalls.
If I change the code slightly (by removing some other tests), the loops fall on better alignment and run as fast as the <b>bitwise and</b>.

### bitwise xor
Good.
<br>

### Other sizes and unsigned values
The 8, 16, 64 bit and unsigned values are very similar to int32_t but are also hitting unaligned
loop problems that make analysis of optimizations a little more difficult (without reading all the assembly).

### Conclusions for integer loop invariants
<ul>
<li>With the -O3 flag for optimization, <b>LLVM</b> is usually moving integer loop invariant calculations out of inner loops.</li>
<li><b>LLVM</b> does not optimize loop invariant integer subtracts. <i>[ LLVM 2.3 fixes this problem - ccox ]</i></li>
<li><b>LLVM</b> is not aligning loops and can have code position dependent slowdowns on some processors. <i>[ LLVM team is working on it - ccox ]</i></li>
</ul>


<hr>
## Floating point math

<pre>
test                        description   absolute   operations   ratio with
number                                    time       per second   test0

 0                 "float variable add"   3.84 sec   416.67 M     1.00
 1       "float multiple variable adds"   6.20 sec   258.06 M     1.61
 2            "float variable subtract"   3.85 sec   415.58 M     1.00
 3  "float multiple variable subtracts"   6.20 sec   258.06 M     1.61
 4            "float variable multiply"   3.84 sec   416.67 M     1.00
 5 "float multiple variable multiplies"   4.72 sec   338.98 M     1.23
 6              "float variable divide"  18.57 sec    86.16 M     4.84
 7    "float multiple variable divides"  53.81 sec    29.73 M    14.01
 8      "float multiple variable mixed"   3.85 sec   415.58 M     1.00

test                         description   absolute   operations   ratio with
number                                     time       per second   test0

 0                 "double variable add"   3.84 sec   416.67 M     1.00
 1       "double multiple variable adds"   6.23 sec   256.82 M     1.62
 2            "double variable subtract"   3.84 sec   416.67 M     1.00
 3  "double multiple variable subtracts"   6.22 sec   257.23 M     1.62
 4            "double variable multiply"   3.84 sec   416.67 M     1.00
 5 "double multiple variable multiplies"   4.17 sec   383.69 M     1.09
 6              "double variable divide"  18.58 sec    86.11 M     4.84
 7    "double multiple variable divides"  53.80 sec    29.74 M    14.01
 8      "double multiple variable mixed"   3.85 sec   415.58 M     1.00
</pre>

<b>LLVM</b> fails to completely optimize even a single case.
But the assembly dump shows that it does partly optimize the mixed math case similar to the way it did for integers.

### Conclusions for floating point loop invariants
<ul>
<li>With the -O3 flag for optimization, <b>LLVM</b> is rarely moving floating point loop invariant calculations out of inner loops.</li>
<li>NOTE: LLVM doesn't do the FP optimizations because the LLVM architect believes it would affect the result of the FP operations.</li>
</ul>
