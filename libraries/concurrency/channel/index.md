---
layout: library
title: Channels
tags: [library]
brief: CSP channels that allow to greate processing graphs that execute the tasks on dedicated executors.
description: |
     This library implements [CSP](https://en.wikipedia.org/wiki/Communicating_sequential_processes) channels. With channels it is possible to realize processing graphs. Channels consist of a sending and a receiving part. Each receiver has an attached process that gets executed when a value is send through. It is possible to split, join, zip and merge channels. Each processing node is associated with a [process](process/index.html).
---
