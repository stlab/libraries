---
title: Adding an Adobe internal client
layout: page
tags: [legacy]
comments: true
---
This is the steps I use to add an Adobe client to ASL. These are executed on a Mac with bash.

```
# Adding an ASL client

# check in their form

export P4PORT=boxer.corp.adobe.com:1770
p4 add //asl_corp/forms/completed/using_<product>_<version>.pdf
p4 submit

# add the user to the asl-users mail list (sends welcome message).

open -a Safari "http://mlm.corp.adobe.com/cgi-bin/majordomo?module=modify"
```
