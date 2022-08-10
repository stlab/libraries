---
title: Configure Perforce client for two servers
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Configuring_Perforce_client_for_two_servers
---
Here is one example of a strategy for working with perforce to merge some of the contents of
depots on two separate servers into one directory tree in a client workspace.
Suppose, for example, that we are working with two depots names //submission, one on a
perforce server named opensource, and the other on a server called boxer. Further, we want to take
part of the contents of boxer and merge it into the directory synced from opensource. In particular
we want //submission/third_party from boxer to behave as if it came from opensource.

1) create client specs which map files to the right place on the client machine.

<pre>
### opensource clientspec
//submission/... //opensource-client-spec/C:/opensource/submission/...

### boxer clientspec
//submission/... //boxer-client-spec/C:/boxer/submission/...
//submission/third_party/... //boxer-client-spec/C:/opensource/submission/third_party/...
</pre>

2) It can be quite helpful to use the p4config mechanism. Instead of the single-server strategy of
setting the P4PORT and P4CLIENT environment variables, one sets the variable P4CONFIG. It is
set to the name of a text file that may be present on disk. I use the name p4config. Now, when a
perforce command is issued from a given working directory, p4 searches for a p4config file in the
current directory, or if not found in successive parent directories. Once a file is found p4 looks
inside for the setting of any number of variables. This is where I set P4PORT and P4CONFIG. In
the current example:

<pre>
### C:/opensource/p4config contains:
P4PORT=opensource.adobe.com:10666
P4CLIENT=mmarcus-pc
P4USER=mmarcus

### C:/opensource/submission/third_party/p4config (and C:/boxer/p4config) contains:
P4PORT=boxer.corp.adobe.com:1770
P4CLIENT=mmarcus-pc
P4USER=mmarcus
</pre>

That's it. Now if I cd to C:/opensource/submission and p4 sync I'll get files from opensource, but if I cd to C:/opensource/submission/third_party the files will come from boxer.
