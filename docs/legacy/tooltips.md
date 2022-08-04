---
title: Tooltips
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Tooltips
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Basic Description

Tooltips are a visual hint provided to the user when they hover the mouse over a user interface element. On most platforms this is in the form of a small yellow-ish window containing descriptive text.

Tooltips are available for some widgets starting in ASL 1.0.18.

## Semantics / Rationale

Tooltips are not really a standalone entity; rather, they are an augmentation to preexisting widgets.

## Requirements

A tooltip for a widget is defined using the <code>alt</code> tag in a layout. For example:

```
my_button(name: "Put them in the iron maiden", alt: "Execute them!");
```

## API

The API is implementation-defined on a per-platform basis; in the case of Mac OS X and Windows XP, this can be reduced to a single API call that augments a control/window with tooltip metadata.

## Other Issues

none.

## Related Work

none.
