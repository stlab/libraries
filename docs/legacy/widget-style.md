---
title: Widget Style
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Widget_Style
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Basic Description

Some of my custom widgets have appearance attributes that can be set in the parameters dictionary (simple things like color and if the corners should be rounded). It might be interesting if I could create "named styles" that could be applied to these widgets (and to ui-core widgets eventually), then I could say "when this happens, that widget should go red".

A concept of style could also be used to finally eliminate bevel_button (note that this might not be how style should be represented...):
* button_beveled -> button( style: beveled, ... )
* checkbox_beveled -> checkbox( style: beveled, ... )
* radio_button_beveled -> radio_button( style: beveled, ... )
* popup_beveled -> popup( style: beveled, ... )

## Semantics / Rationale

_What is the behavior of the widget? What data structure is it intended to model, or what rationale is there for needing it?_

## Requirements

_What are the fundamental requirements this widget must meet in order to solve the problem for which it was created?_

## API

_What is the programming interface to the widget?_

## Other Issues

_What other issues will need consideration?_

## Related Work

_What additional code must be written to support this widget? What platform implementations / APIs can be leveraged when implementing this widget?_
