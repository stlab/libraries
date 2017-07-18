---
title: Updating the public website
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Updating_the_public_website
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Required Software
Programs you will need (both from a command line):
* rsync (a *nix utility that comes with Mac OS X and can be installed for cygwin)
* Browser (FireFox, IE, etc.)

## Directions
1. Build the doxygen documentation on your computer, making sure the warning file is empty at the end
1. Open a command line terminal
1. Assuming your ASL sources are stored at ~/opensource/, execute the following rsync command:
    * <pre>rsync -az --progress --stats ~/opensource/submission/adobe/documentation/html/ webprod.macromedia.com:/data/www/sites/stlab-staging.adobe.com/htdocs ; echo "rsync done."</pre>
    * You may get some permission warnings for some directories like "." -- those are ok
1. When the rsync is complete, open your browser and head to
    * http://webpush.macromedia.com/stlab.adobe.com/
    * Note that you may have to log in with you LDAP information
1. Once the page has loaded (it could take a while given the volume of pages in a directory) hit the "Check all push boxes" button at the top of the page.
    * If you are uploading directories with content make sure the "Push all subdirectories of checked items" checkbox is checked as well.
1. Hit the "Push and/or Delete!" button
1. After about 10-15 minutes you should see the public web site updated.
