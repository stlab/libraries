---
title: Prof. Lumsdaine Collaboration
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Prof._Lumsdaine_Collaboration
---
#### Contents
{:.no_toc}
1. contents
{:toc}

### Collaboration Participants and Affiliation:
* Adobe Collaborator: Sean Parent
* University Collaborator: Andrew Lumsdaine
* Collaboration Project Name: Declarative Models for Software Composition
* University Name: Indiana University
* Proposed Duration of Collaboration:

### Nature of Collaboration:

### Objectives:

Large-scale software systems today are typically built by composing
together smaller-scale objects (or components).  This composition
process gives rise to large-scale software structures (implicit
algorithms and implicit data structures) that are not well understood.
Recent work from Adobe indicates that declarative models allow the
composition process to be effected in a manner that can be reasoned
about in a principled fashion.

Numerous questions remain however.  For example:
* With current composition technology, what are the structures formed by large-scale software systems and what are the invariants that govern them?
* How can these structures and invariants be described in a form upon which the computer can operate?
* How can algorithms which transform these structures be modeled such that the valid set of transformations for a given state can be determined?
* What algorithms can be applied to the structures so they are self-correcting when invariants are violated?
* How can both the transformation and correcting algorithms be described such that can be applied generically?
* How do these ideas apply in the case of communicating sequential  processes?

Answering these questions, and firmly establishing the theory and
practice of declarative approaches to software composition, will be a
significant undertaking.  Since this approach goes hand in glove with
generic programming, it will be an important part (if not the most
important part) of the research agenda for my lab for years to come.
To launch this new research initiative, I propose to work closely with
Adobe during my sabbatical leave on particular projects in this area.
This collaboration will result in a deep understanding of the
foundational issues in declarative composition of software components.

One project will involve the property model library (Adam).

One way of defining a UI is that it is a system for assisting a user
to select a function and provide a valid set of parameters to that
function.  Given that a function is selected, there remains the
problem of assisting the user in providing a valid set of
parameters. If we define validity as a predicate, P, on the set of
parameters, X, then "assisting" is a function, F, which is a
transformation of X, such that P(F(X)) is true.

There are different forms of F that we can specify as "assisting" -
for example assume we are given an X such that P(X) is true (we are
already in a valid state) and the user changes a single value of X,
say x[i]. Then we would like a function, F, such that P(F(X)) is true
and F(X)[i] is equal to the user set x[i]. This is a simple form of
correction. For this we would also like the system to be idempotent -
such that if P(X) then F(X) == X.

Another form of assistance is prediction - if we know the
post-conditions of the function for which X describes the parameters
then we can incorporate the result into X and the postconditions into
the predicate.  In this case P(X) implies both a valid set of
preconditions but also an accurate prediction of the result of our
function.  Using the same notion of changing a single value in X, but
in this case where the value is part of the result, we can update the
parameters to get the desired result.

Important questions for the property model approach to this problem
include:
* How does one describe F()?
* What are the forms of F() that are solvable?
* What is the space of solvable definitions?

Generic programming and generic libraries will also constitute an
important part of this collaboration.  My lab has been making
contributions to the upcoming C++0x standard, particularly in the area
of concepts.  While the addition of concepts to C++ will significantly
improve the support for generic programming and generic libraries, it
will also require a change in the way generic programming is taught
and practiced.  Accordingly, we intend to design a teaching subset of
C++0x for generic programming and to develop corresponding teaching
material.  In addition, tool support in the form of a prototype
validator for this subset will be developed.

### Milestones & Completion Dates:
* Sep 1, 2007: Kickoff
* Dec 1, 2007:
* Mar 1, 2008:
* Jun 1, 2008:
* Sep 1, 2008:


### Expected Results and Artifacts:
* Improvements to, and an improved understanding of the property model library as well as related libraries to be developed as part of the Adobe Source Libraries.
* Grant proposal submitted to National Science Foundation Science of Design program. Topic: Declarative composition of generic software components.
* Grant proposal submitted to National Science Foundation Computer Systems Research program. Topic: Declarative composition of sequential communicating processes.
* Publication of results to be submitted to appropriate conferences and/or journals (e.g., OOPSLA, ECOOP, LCSD, PLDI, POPL).
* Prootype C++0x teaching subset validator.

### Requested Gift Funding (don't post on wiki - handle via e-mail):

### IP information and agreement

Currently no Adobe confidential information is planned to be
disclosed. Currently no IU confidential information is planned to be
disclosed.

Adobe code which is available under an open source license (see
<http://stlab.adobe.com/licenses.html>) will be
used. Contributions to this project from IU will be released under the
MIT open source license.
