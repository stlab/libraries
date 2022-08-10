---
title: Better Edit Number Widget
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Better_edit_number_Widget
---
#### Contents
{:.no_toc}
1. contents
{:toc}

This is a place to collect thoughts on building a better edit_number widget.

## Background

Editing numbers is fairly fundamental within a UI. Numbers, always have some semantic interpretation - they mean something. Often the semantics of a number are so specialized that it deserves a separate widget - a good example is a data/time widget.

However, the a common use of numbers in UI is to represent measured values such as distance, weight, opacity, intensity, count, etc. Such measured values should be treated consistently within the UI (even here, we only have a guideline - selecting an angle is an example of a measured value which likely deserves a custom widget).

This page lists the desired attributes of a good edit_number widget - some of these attributes apply more broadly to other widgets but edit_number is such a commonly used widget that exploring it is a good way to cover many issues.

The issues to consider in the design of this one widget are all quite entangled - choices made in one area impact another. This page doesn't attempt to design a solution - only to organize and list the issues.

## Units and Related Values

Numbers are an interpretation of ''something''. Any physicist will tell you that all numbers have a unit associated with them. A single number can often be represented in multiple units - such as inches and cm. Within a UI a user should be able to change the units a value is displayed in, as well as change the unit a value is interpreted in. There are two distinct operations "show me this 3cm value as inches" and "interpret this 3 as 3 inches." Often units are related to other values - for example the inches width in an image is related to the resolution of the image - it's percentage width may be related to a previous size. The size a video will take up on disk is a function of many variables (time, frame size, compression). Depending on the unit, the related values may change. This is why in the current edit_number allows separate binding per unit.

## Valid Ranges, Increments, and Scales

Numbers are often constrained to ranges and increments and an appropriate view for a range may be a non-linear scale such as logarithmic. Making the valid range discoverable, handling of invalid entries (see Filtering vs. Validation), and representation for the scale and increments for analog input are all issues. The range and increments should also come into play in the choice for the width of the edit_number field.

There should also be a mechanism to "bump" a value up and down by a meaningful increments (which may not be the same as a limiting increment on the value) - bumping values should be accessible from both the keyboard an mouse.

## Filtering vs. Validation

## Analog Representation

## Accessibility

## Interactivity and Commit

## Display vs. Disabled

## Locking, Recording, and Related Values

## Expressions
