---
title: Welcome to the stlab.adobe.com Wiki
tab: Legacy
layout: page
redirect_from: /legacy/index.php/Main_Page
icon: pagelines
---

_These pages have been migrated from stlab.adobe.com/wiki for preservation. They may eventually be revised and moved into a more appropriate location on the site._

Adobe's Software Technology Lab web site is the home for the Adobe Source Libraries (ASL).

ASL provides peer-reviewed and portable C++ source libraries. The libraries are intended to be widely useful, leveraging and extending both the C++ Standard Library and the Boost Libraries.

stlab.adobe.com is maintained by Sean Parent, Mat Marcus and Foster Brereton.

This portion of the web site is place where users of the Adobe Source Libraries can collaborate on everything that pertains both to their development and to the development of technologies that leverage it.

## Contents
{:.no_toc}
1. contents
{:toc}

## Documentation, Design and Development

* [Supplementary ASL Documentation](supplementary-asl-documentation.html)
* [Current Design Rationale](current-design-rationale.html)
* [Future Ideas and Designs](future-ideas-and-designs.html)
* [Papers and Presentations](papers-and-presentations.html)
* [Troubleshooting](troubleshooting.html)
    * [MSVC 8.0 "Secure" Library Issues](troubleshooting.html)

## Internals

* [GitHub Migration and Status](github-migration-and-status.html)
* [Release Process](release-process.html)
* [Configuring Perforce client for two servers](configure-perforce-client-for-two-servers.html)
* [Updating the public website](updating-the-public-website.html)
* [Adding a user to the ASL Wiki](adding-a-user-to-the-asl-wiki.html)
* [Accessing Internal Logs](accessing-internal-logs.html)
* [Build and Debug Tips](build-and-debug-tips.html)

## Other

* [Build Matrix](build-matrix.html)
* [Perforce Open Source License](perforce-open-source-license.html)
* [EMacs Questions](emacs-questions.html)

## Relate Your Success

The Adobe Source Libraries have been used with success within Adobe applications to increase robustness and correctness of code. As developers of an open source project, we are confident that there are other success stories outside of Adobe as well. We encourage you to contribute a success story of your own:

* What is the name of your project?
* What is the purpose of the project?
* What led you to choose ASL?
* Was ASL used from the start of the project? If not, how did its inclusion impact your project? How does using ASL affect your project development as a whole?
* How large is your project?
* How would you rate the success of your project?
* Is there any other interesting information worth sharing?

Submissions will be included in a success stories web page on the opensource.adobe.com site. You can submit a success story by emailing Foster Brereton at his adobe.com email address (fbrereto).

## Complete list of pages

<dl class='posts'>
{% for p in site.pages %}
    {% if p.tags contains 'legacy' and p.url != page.url %}
        <dt><a href="{{ BASE_PATH }}{{ p.url }}">{{ p.title | markdownify }}</a></dt>
    {% endif %}
{% endfor %}
</dl>
