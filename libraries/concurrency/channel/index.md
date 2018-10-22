---
layout: library
title: Channels
tags: [library]
brief: This library implements the channel model of interprocess communication.
library-type: library
description: |
     [Channels](https://en.wikipedia.org/wiki/Channel_(programming)) are one model of interprocess communication that has its roots in [communicating sequential process](https://en.wikipedia.org/wiki/Communicating_sequential_processes) (CSP).
     With channels it is possible to realize processing graphs, that can be used for more than one execution, compared to a graph that is setup with futures.
     Channels consist of a sending and a receiving part. Each receiver has an attached process that gets executed when a value is send through. It is possible to split, zip, zip_with, or merge channels. Each processing node is associated with a [process](process/index.html).
---
