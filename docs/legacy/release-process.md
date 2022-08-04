---
title: Release Process
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Release_Process
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## setup
<pre>
# REVISIT (sparent) : Later steps assume that bjam is pre-built and path set - should not assume that.

export ADOBE_ROOT=`p4 where //adobe_platform_libraries/... | perl -MFile::Basename -ane 'print dirname dirname $F[2]'`
</pre>

## bump the version number in <code>//adobe_source_libraries/adobe/config.hpp</code>
<pre>
CONFIG=adobe_source_libraries/adobe/config.hpp
p4 edit //$CONFIG
$EDITOR $ADOBE_ROOT/$CONFIG
p4 submit
</pre>
## Build and test all mac targets
<pre>
# win users can ssh into their mac machine
# this step assumes that bjam is installed
export ADOBE_ROOT=`p4 where //adobe_platform_libraries/... | perl -MFile::Basename -ane 'print dirname dirname $F[2]'` && \
cd $ADOBE_ROOT/adobe_platform_libraries && \
bjam -j3 debug release && \
cd $ADOBE_ROOT/adobe_platform_libraries/xcode_ide/begin.xcodeproj && \
xcodebuild -configuration Debug   && \
xcodebuild -configuration Release  && \
#also build 64bit ASL debug/release && \
cd $ADOBE_ROOT/adobe_source_libraries && \
bjam -j3 debug address-model=64 && \
bjam -j3 release address-model=64 architecture=combined
#end
</pre>

## Build and test all windows targets
<pre>
 # mac users can ssh into their windows machine
 # this step assumes that bjam is installed, in path, that perl is installed properly
 # and that gcc-4.3.0 is also installed and recognized by bjam
 export ADOBE_ROOT=`p4 where //adobe_platform_libraries/... | perl -MFile::Basename -ane 'print dirname dirname $F[2]'` && \
 perl $ADOBE_ROOT/adobe_platform_libraries/tools/build_win.pl

## on a vista 64 system:
 export ADOBE_ROOT=`p4 where //adobe_platform_libraries/... | perl -MFile::Basename -ane 'print dirname dirname $F[2]'` && \
 cd $ADOBE_ROOT/adobe_source_libraries && \
 bjam -j2 debug release address-model=64 architecture=x86
 #end




</pre>

## Rev codex for Boost
<pre>
## skip this whole section if boost is unchanged
## edit the next four fields
## REVISIT (mmarcus) : Calculate these from current date and $ADOBE_ROOT/adobe_source_libraries/adobe/config.hpp
## this assumes that perl is working properly and that the required cpan modules have been installed

##BOOST_VERSION should be incremented by one from the previous VersionInfo.xml version_build field
BOOST_VERSION=2
ASL_DATE=20070503
LDAP_NAME=ldap_username
LDAP_PASSWD=passwd
cd $ADOBE_ROOT/boost_libraries/adobe_information
p4 edit VersionInfo.xml
perl $ADOBE_ROOT/adobe_source_libraries/tools/boost_codex.pl $BOOST_VERSION $BOOST_DATE
perl $ADOBE_ROOT/adobe_source_libraries/tools/post_boost_build.pl $BOOST_VERSION $LDAP_NAME $LDAP_PASSWD
p4 submit
</pre>
Output should be two key/value pairs. Check the results at the Boost Codex Page (http://matrix.corp.adobe.com/Codex/webcontent/index.jsp?action=build_history&product=Boost&product_version=1_34_1)

## Rev codex for Intel Thread Building Blocks
<pre>
## skip this whole section if intel tbb is unchanged
## edit the next four fields
## REVISIT (mmarcus) : Calculate these from current date and $ADOBE_ROOT/adobe_source_libraries/adobe/config.hpp
## this assumes that perl is working properly and that the required cpan modules have been installed

##INTEL_TBB_VERSION should be incremented by one from the previous VersionInfo.xml version_build field
INTEL_TBB_VERSION=2
ASL_DATE=20070503
LDAP_NAME=ldap_username
LDAP_PASSWD=passwd
cd $ADOBE_ROOT/intel_tbb_libraries/adobe_information
p4 edit VersionInfo.xml
perl $ADOBE_ROOT/adobe_source_libraries/tools/intel_tbb_codex.pl $INTEL_TBB_VERSION $ASL_DATE
perl $ADOBE_ROOT/adobe_source_libraries/tools/post_intel_tbb_build.pl $INTEL_TBB_VERSION $LDAP_NAME $LDAP_PASSWD
p4 submit
</pre>
Output should be two key/value pairs. Check the results at the Intel Thread Building Block Codex Page (http://matrix.corp.adobe.com/Codex/webcontent/index.jsp?action=build_history&product=Intel%20Thread%20Building%20Blocks&product_version=tbb20_014oss)

## Rev codex for ASL and APL

<pre>
$ADOBE_ROOT/adobe_platform_libraries/tools/update_adobe_codexes.pl date (format: 20080128) ldap-username ldap-password
p4 submit
</pre>

expected output from the first command:

<pre>
building codexes for version 1.0.35
executing p4 edit /adobe_root/adobe_source_libraries/tools/VersionInfo.xml in /adobe_root/adobe_source_libraries/tools
//adobe_source_libraries/tools/VersionInfo.xml#5 - currently opened for edit
1executing perl -w asl_codex.pl 1.0.35 20080128 adobe_source_libraries_1.0.35 in /adobe_root/adobe_source_libraries/tools
executing perl -w post_build.pl 1.0.35 mmarcus Brea%m14 in /adobe_root/adobe_source_libraries/tools
key: result, value: 97281.
key: result, value: 97282.
executing p4 edit VersionInfo.xml in /adobe_root/adobe_platform_libraries/tools
//adobe_platform_libraries/tools/VersionInfo.xml#5 - currently opened for edit
executing perl -w apl_codex.pl 1.0.35 20080128 adobe_platform_libraries_1.0.35 in /adobe_root/adobe_platform_libraries/tools
executing perl -w post_build.pl 1.0.35 mmarcus Brea%m14 in /adobe_root/adobe_platform_libraries/tools
key: result, value: 97283.
key: result, value: 97284.
</pre>

## Semi-automated from here
* rev <code>$ADOBE_ROOT/adobe_source_libraries/read_me.html</code> with current version numbers for "automated build" instructions.
* Update <code>$ADOBE_ROOT/adobe_source_libraries/tools/patch_boost.sh</code> if a new version of boost has been released. Also update boost version numbers in $ADOBE_ROOT/adobe_source_libraries/documentation/html/asl_readme.html and <code>$ADOBE_ROOT/adobe_platform_libraries/tools/verify.sh</code>. Furthermore, make sure that the boost patch file name is updated, e.g. <code>$ADOBE_ROOT/adobe_source_libraries/tools/boost_1_44_0_patches.txt</code>.
* Update the boost patch file contents e.g. <code>$ADOBE_ROOT/adobe_source_libraries/tools/boost_1_44_0_patches.txt</code> . For example, under win32, generation of the patch file might look like:
<pre>
## given a clean copy of boost in $ADOBE_ROOT/diffable_boost_1_44_0
cd  $ADOBE_ROOT/adobe_source_libraries
diff -wur --unidirectional-new-file -x status -x adobe_information -xp4config -x bin.ntx86 -x bin.cygwinx86 -x bin.v2 ../diffable_boost_1_44_0 ../boost_libraries > ./tools/boost_1_44_0_patches.txt
</pre>
Note, however, that the diffs for boost filesystem's wrong_line_end test may be spurious (depending on perforce line ending settings). As of 9/2008 that file has *not* been patched. Also be sure not to pick up p4config files or to add them to p4. Note: if boost patches was updated externally by the opensource community then those patches should be applied to the internal boost first.
* Update the $ADOBE_ROOT/adobe_source_libraries/release_notes.txt with the latest revision information-- <code>p4 changes -l -m N //adobe_source_libraries/... //adobe_platform_libraries/... </code>is helpful
* Update main news page with new highlights: $ADOBE_ROOT/adobe_source_libraries/documentation/structure/news.dox
* Update all external docs:
<pre>
#### editing the news file

### open the release notes

p4 changes -l -m 150 //adobe_source_libraries/... //adobe_platform_libraries| bbedit

RELEASE_NOTES=//adobe_source_libraries/release_notes.txt
p4 edit $RELEASE_NOTES
bbedit `p4 where $RELEASE_NOTES | sed -e 's/[^ ]* [^ ]* //'`

### open and edit the news file

NEWS=//adobe_source_libraries/documentation/structure/news.dox
p4 edit $NEWS
bbedit `p4 where $NEWS | sed -e 's/[^ ]* [^ ]* //'`
	# update the news file based on the release notes.
p4 submit

#### updating the documentation
cd $ADOBE_ROOT/adobe_source_libraries/documentation/html
$ADOBE_ROOT/adobe_source_libraries/tools/doc_update.pl


### review changes, revert unchanged files, and submit

cd html
explorer index.html

p4 submit
</pre>

* Integrate from //adobe_source_libraries to //source_release, and same for platform:

<pre>
p4 integrate -t -i -Dt -Ds -b source_and_platform_release
p4 resolve -at
p4 submit
</pre>
* Upload to SourceForge 10 files (sftp: frs.sourceforge.net using sftp with your sourceforge credentials. See https://sourceforge.net/apps/trac/sourceforge/wiki/Release%20files%20for%20download . No two files in your entire release library can have the same name). Create the source tarball/zip using the perl script
<code>
 perl $ADOBE_ROOT/adobe_platform_libraries/tools/package_distros.pl
</code> on *each* platform (don't forget to sync on secondary platform first) to create the files in the local directory /distros, for uploading to sourceforge. You will need at least the compressed Begin application from your secondary platform. The other files can be uploaded from your primary platform (as long as your perforce client spec uses LineEndings of "share"). The files to be uploaded are
    * Adobe Begin binaries
        * <code>asl_1.0.XX_begin_mac.tgz</code> (includes app, dependencies, and examples folder--see the <code>package_distros.pl</code> script for details.)
        * <code>asl_1.0.XX_begin_win.zip</code> (includes app, dependencies, runtime libs, and examples folder--see the <code>package_distros.pl</code> script for details.)
    * pmeval binaries
        * <code>asl_1.0.XX_pmeval_mac.tgz</code> (includes app, and examples folder--see the <code>package_distros.pl</code> script for details.)
        * <code>asl_1.0.XX_pmeval_win.zip</code> (includes app, runtime libs, and examples folder--see the <code>package_distros.pl</code> script for details.)
    * Net setup scripts
        * <code>asl_1.0.XX_net_setup.sh</code>
        * <code>asl_1.0.XX_net_setup.bat</code>
    * Source Distributions
        * Name: <code>asl_1.0.XX.tgz</code>
        * Name: <code>asl_1.0.XX.zip</code>
        * Name: <code>apl_1.0.XX.tgz</code>
        * Name: <code>apl_1.0.XX.zip</code>
* Go to https://sourceforge.net/project/admin/explorer.php?group_id=132417
    * Click <code>[Add Release]</code> next to <code>adobe-source</code>
    * Follow basic form to add details to the release (release name: (e.g.) 1.0.31)
        * Copy the most recent release notes from $ADOBE_ROOT/adobe_source_libraries/release_notes.txt to "Changelog" field. Open $ADOBE_ROOT/adobe_source_libraries/read_me.html, save as text, paste text file into the release notes field. Select "preserve my preformatted text"
            * Make sure you hit the "preserve formatting" checkbox at the bottom
        * Add the files from the FTP portion of the form to the release, and update all their types
        * Go to "Notice" section at the bottom, hit "I'm sure", and post it.
* Update the utility scripts (p4checkout) if need be
* Update stlab.adobe.com with new documentation: once you have rsynced up the files, by running opensource_rsync.sh, you go to http://webpush.macromedia.com/stlab.adobe.com/ , provide LDAP info, Push the "Check all push boxes" button, then go through the list and uncheck the push boxes for the directories (amg/, gil/ and performance/). When that's ready, hit the "Push and/or Delete!" button at either the top or bottom of the page
* Test the net setup scrips on both platforms. It's good to choose paths with spaces in them for robustness. Note: sometimes it takes SF a bit of time ot propagate the uploaded files to the mirrors/download areea.
* Update internal website with new documentation (not necessary most releases, unless changed):
<pre>
  build the internal docs so you have a complete html/ directory. That is,
  cd $ADOBE_ROOT/asl_corp/documentation/
  p4 edit html/...
  rm html/*.html # but not the gifs/pdfs
  doxygen doxyfile
  p4 add html/*
  p4 revert -a html/...

  cd html
  explorer index.html
  #if all ok:

  p4 submit

  Make a tarball of $ADOBE_ROOT/asl_corp/documentation/html directory:

   cd $ADOBE_ROOT/asl_corp/documentation
   rm html.tgz
   tar -czf html.tgz html

  FTP that tarball up to the internal doc staging server, with the following information:

  ftp ihostunix.corp.adobe.com # use LDAP credentials
  cd /www/isproot/intranet1/corp/stlab
  bin
  put html.tgz
  quit

  Once the file is uploaded,
  telnet ihostunix.corp.adobe.com # use LDAP credentials
  bash
  ./bin/stlab.sh  #remotely expands the tarball and moves the new docs into place.
</pre>

### Posting Docs to stlab.adobe.com

Info on connecting to the staging server here: http://webops.corp.adobe.com/wiki/index.php/Web-stage_FTP_process
*  Use an FTP program such as Transmit to copy the updated docs to the staging directory

Webpush interface here: https://web-stage.corp.adobe.com/webpush/stlab.adobe.com/
*  once the docs are staged hit the "check all push boxes" button, then the "push and/or delete" button

### Scripts
#### opensource_rsync.sh
<pre>
#!/bin/bash

## Setup the argument components
export masterDir="/data/www/sites/stlab-staging.adobe.com/htdocs"
export rsync="/bin/rsync"
export ssh="--rsh=/usr/bin/ssh"
export recursive="--recursive"
export preserve="--times -og --links"
export prog="--progress"
export stats="--stats"
export delete="--delete"
export remoteUser="ldapusername"
export server="webprod.macromedia.com"
export args="$prog $stats $ssh $recursive $preserve"
rsync $args /adobe_root/adobe_source_libraries/documentation/html/ ${remoteUser}@${server}:${masterDir}/
</pre>

#### stlab.sh
<pre>
#!/bin/bash
cd /www/isproot/intranet1/corp/stlab
tar -xzvf html.tgz
echo 'changing permissions...'
chmod -R a+rx html
echo 'relocating directories...'
mv www www.old
mv html www
rm -rf www.old
echo 'done!'
exit
</pre>

#### stlabconnect
<pre>
rsh -l ldapusername ihostunix.corp.adobe.com
</pre>
stlab.sh is actually a script that lives on the internal documentation staging server. Once the html tarball has been uploaded to the server, run this script to expand the tarball and swap it into place, removing the old documentation directory upon completion.


* Notify version tracking websites:
    * http://www.google.com/codesearch/addcode
* Post a news item on our SourceForge home page: log in to our project page with your sf.net username/pass, and you'll get an "Admin" menu on the site -- click the news item in that menu, and in that interface you should see a link that says something like "Post New News". Get the news from $ADOBE_ROOT/adobe_source_libraries/documentation/html/index.html
* Update the internal perforce server: Push the new releases (from boxer) //source_release/... and //platform_release/... on Perforce
<pre>
    p4 integrate  -t -i -Dt -Ds -b source_and_platform_release
    p4 resolve -at
    p4 submit
</pre>
* Update labels for the release (asl, apl, intel_tbb, and boost) (adobe_source_1_0_XX)
<pre>
   #eg to go from 1.0.35 to 1.0.36:
   p4 label -t adobe_source_1_0_35 adobe_source_1_0_36
   # in the ensuing dialog we add a revision field and insert the head rev, e.g.:
     Revision:
       @4082
</pre>
  Check that it worked by examining the label in P4V, comparing it to the previous label
* update https://zerowing.corp.adobe.com/display/stlab/Changelist+Numbers+For+ASL+Release
* Update the [[Annotated List of Boost Patches]]
* Mail an announcement to asl-users@adobe.com
* Mail an announcement to adobe-source-devel (sf.net)

## Sean's Worksheet For Adobe Internal Release

<pre>
##### branch spec for integrating adobe portion of submission to source_library

p4 sync

# update the version number
p4 edit ~/development/projects/submission/adobe/config.hpp
bbedit ~/development/projects/submission/adobe/config.hpp

# remove the old documentation
rm -r ~/development/projects/submission/adobe/documentation/external/html/*
rm -r ~/development/projects/submission/adobe/documentation/internal/html/*
p4 delete //submission/adobe/documentation/external/html/...
p4 delete //submission/adobe/documentation/internal/html/...
p4 submit

# build the docs and check them

cd ~/development/projects/submission/adobe/documentation/external
doxygen documentation.doxygen
bbedit ./doxygen_support_files/doxygen_warn_log.txt
open -a Safari html/index.html

cd ~/development/projects/submission/adobe/documentation/internal
doxygen adobe.dox
bbedit doxygen_warn_log.txt
open -a Safari html/index.html

# add the built documentation
p4 add ~/development/projects/submission/adobe/documentation/external/html/*
p4 add ~/development/projects/submission/adobe/documentation/internal/html/*
p4 submit

# check the branch spec
p4 branch adobe_release

p4 integrate -1 -t -d -i -b adobe_release | bbedit
p4 resolve -at | bbedit
p4 submit | bbedit

# update the label

p4 label -t adobe_source_1_0_11 adobe_source_1_0_12
p4 labelsync -l adobe_source_1_0_12 //source_library/... //submission/... | bbedit
p4 label adobe_source_1_0_12 # lock the label

# check the build

rm -r ~/development/projects/source_library/*
p4 sync -f //source_library/...@adobe_source_1_0_11 | bbedit

~/development/projects/source_library/adobe/bin/build.sh

</pre>
