---
title: Representational Equality
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Representational_Equality
---

[_This page is being used to collaborate on a paper. You are welcome to read and comment on it, but consider it a work in progress, not a final product._]

**Title:** Representational Equality

**Authors:** Sean Parent, Alex Stepanov

#### Contents
{:.no_toc}
1. contents
{:toc}

## Abstract

## Introduction

## Body

Definitions -

identical
: I(a, b) - same instance, &a == &b
representationally equal: R(a, b) - representation is equal
logically equal: L(a, b) - all corresponding parts are equal

locational less than
: std::less<T*>(&a, &b)
representationally less than:

I ==> R ==> L

I - complexity
: small, constant time (pointer comparison)

R - complexity
: average small constant time, worst case O(area)

L - complexity
: may be significant - time to find corresponding parts

identical
: &a == &b

representationally equal
: R(a, b) ==>

Desired attributes for logical less
   symmetric with other operators - esp. + and *

Examples of types where < and/or == is problematic:

float and double - reason is NaN
complex - operator < not defined because of symmetry with operator*()
   solution is provide representational_less
priority_queue - operator == and operator < not defined because of complexity and resource consumption

## Proposal

## Related Work

## Acknowledgments
