---
title: Dynamic Libraries
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Dynamic_Libraries
---
[_This page is being used to collaborate on a paper. You are welcome to read and comment on it, but consider it a work in progress, not a final product._]

__Title:__ Dynamic Library Support for C++

__Authors:__ Sean Parent, Mat Marcus

#### Contents
{:.no_toc}
1. contents
{:toc}

## Abstract

## Introduction

## Body
### Basic Use Cases
#### Separate Compilation (Revision Locked)
#### Upgradable Component (Not Revision Locked)
#### Plug-In Component
### Dynamic Library Issues
#### Standard ABI
#### Name Mangling
#### RTTI
#### Exception Handling
#### Replacement Operators
#### Symbol Visibility
#### Static Initialization
#### Object Lifetime
#### Code Lifetime
#### Inline Functions and Template Instantiations
#### Exported Templates
#### Library Versioning
##### Multiple Versions in Same Application
#### Standard Library Symbols and Versioning

#### Issues with New Language Features?

## Proposal

## Related Work

[[1]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1400.html) N1400 Toward standardization of dynamic libraries, Matt Austern

[[2]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1418.html) N1418 Dynamic Libraries in C++ Notes from the Technical Session in Santa Cruz, Oct. 21, 2002, Pete Becker

[[3]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1496.html) N1496 Draft Proposal for Dynamic Libraries in C++ (Revision 1), Pete Becker

[[4]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1976.html) N1976 Dynamic Shared Objects: Survey and Issues, Benjamin Kosnik

[[5]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2013.html) N2013 Versioning with Namespaces, Benjamin Kosnik

[[6]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2117.htm) N2117 Minimal Dynamic Library Support, Lawrence Crowl

[[7]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2073.pdf) N2073 Modules in C++ (Revision 4), Daveed Vandevoorde

[[8]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2074.pdf) N2074 Plugins in C++, Daveed Vandevoorde

## Acknowledgments
