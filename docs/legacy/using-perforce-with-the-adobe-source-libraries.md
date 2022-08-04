---
title: Using Perforce with the Adobe Source Libraries
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Using_Perforce_with_the_Adobe_Source_Libraries
---

The Adobe Source Libraries have migrated away from the SourceForge
CVS/Subversion servers in favor of a Perforce server that is hosted on
stlab.adobe.com. Guest access for this server is enabled, allowing
anonymous users to grab snapshots of the current release sources as well
as the current working set of sources for the upcoming release.

The primary difference between Perforce and other CMS services like CVS
or Subversion is that the state of the client's image of the depot
(repository in CVS parlance) is stored server-side, not derived
client-side. This requires the creation and maintenance of per-user
_clientspecs_ (client path mapping specifications), and _branchspecs_
(file/directory branching specifications). The following steps will
guide you through two mutually-exclusive scenarios. In either case you
will have to download the Perforce client first.

#### Contents
{:.no_toc}
1. contents
{:toc}

## Perforce Depot Location

The Adobe Source Libraries' Perforce depot is at

    stlab.adobe.com:10666

## Downloading Perforce

The Perforce client software is free and can be downloaded for
[Windows](https://www.perforce.com/downloads),
[Mac OS X PPC](https://www.perforce.com/downloads),
[Mac OS X x86](https://www.perforce.com/downloads),
[Cygwin](https://www.perforce.com/downloads),
and a host of
[other platforms](https://www.perforce.com/downloads).

Each operating system at a minimum has a command-line binary available.
For some operating systems there are GUI front end applications
available as well ([Mac OS X](https://www.perforce.com/downloads), [Windows](https://www.perforce.com/downloads)). If you
are following these instructions you will want to download the
command-line client, and place the <code>p4</code> application in a
directory that is in your computer's <code>PATH</code>.

### Scenario 1: Using <code>p4checkout</code>

<code>p4checkout</code> is intended to emulate the CVS
<code>checkout</code> command, grabbing a snapshot of the state of the
ASL depot and copying it to your local computer.

The pros of this method are that it is anonymous and very easy to
execute.

The cons of this method are that there is no permanent record-keeping,
and as such you will not be able to submit any changes to the Adobe
Source Libraries.

This method is recommended for users of the Adobe Source Libraries who
are interested in the state of the depot without having a desire to
contribute directly to the project.

If you are on Windows machine, download the following batch file http://easynews.dl.sourceforge.net/sourceforge/adobe-source/p4checkout.bat

If you are on Mac OS X, Linux, or some other *nix-compatible system,
there is a bash shell script to download: http://easynews.dl.sourceforge.net/sourceforge/adobe-source/p4checkout.sh

When you have this file downloaded (either version), you simply place it
in the directory you would like to serve as the root directory for your
local copy of the ASL depot, and you run it. (You did download a
Perforce client already, didn't you?)

The script will create a temporary (anonymous) client spec, sync the
server's contents to your computer, and delete the client spec from the
server. Please note this is a destructive sync, so any local changes to
files that are to be synced will be overwritten.

Note that this is not enough to get you building ASL yet: you will have
to download and patch Boost,
and then you will be ready.

For more information on the p4checkout utility, pass 'help' as the first
parameter to the script.

### Scenario 2: Using a user-specific clientspec

The second method for interacting with the stlab.adobe.com Perforce
server is the more traditional method of doing source code management in
Perforce. The pros to this method are that you will be able to check in
and check out files directly to the stlab.adobe.com Perforce
server. The cons are that there are several steps required in order to
get up and running. However, once everything is in place, the general
Perforce workflow is very easy. The general steps of this process are:

1. Obtain an account on the stlab.adobe.com Perforce server.
1. Create a clientspec for your local machine
1. Synchronizing with the server

Obtaining a user account can be accomplished by emailing one of the
administrators of the Adobe Source Libraries.

Once you have a user account (and, assuming you have already downloaded
a Perforce client) you can create you client spec by
executing the following from a command line:

    p4 client {nameofclient}

... where {nameofclient} is the client spec name. Note that you can have
as many client specs as you need, on as many machines as you need. When
you execute this some text editing application will launch and you will
see something like the following:

    # A Perforce Client Specification.

    Client:	fbrereto

    Owner:	fbrereto

    Description:
    Created by fbrereto.

    Root:	/Users/fbrereto/opensource/

    Options:	noallwrite noclobber compress unlocked nomodtime normdir

    LineEnd:	local

    View:
      //release/... //fbrereto/release/...
      //submission/... //fbrereto/submission/...

Feel free to change the <code>Root</code> path as you see fit. This will
be the top-level folder in which Perforce will store your local version
of the stlab.adobe.com depots. The <code>Options</code> flags need
not change, but changing "nocompress" to "compress" may speed up network
transmission of files. Finally, save the file and close the text editor
to commit the client spec to Perforce.

The final step in the process is syncing the remote server to your local
computer. This is done in one step:

    p4 sync

Note that this is not enough to get you building ASL yet: you will have
to download and patch Boost, and then you will be ready.

Also note that in order to check in changes to ASL's Perforce depot, you
will need to do so in the sandbox depot. This will require you to create
a branchspec and integrate the sources from the submission depot into
your sandbox. For more information on working with Perforce, the
clientspec, and branchspecs, please see the [Perforce User Guide](https://www.perforce.com/perforce/doc.052/manuals/p4guide/index.html).
