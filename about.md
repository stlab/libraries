---
layout: page
title: About
tab: About
permalink: /about/
---

stlab is the ongoing work of what was Adobe's Software Technology Lab. The Adobe Source Libraries (ASL), Platform Libraries, and new stlab libraries are hosted on [github](https://github.com/stlab).

<h2>Releases</h2>

<table class='definition-table'>
    <tr>
        <td class='decl'>
            stlab/libraries <a href='https://github.com/stlab/libraries/releases/tag/v1.0'>v1.0</a>
        </td>
        <td class='defn'>
            Added the concurrency library, including futures, executors, and channels.
        </td>
    </tr>
</table>

<h2>Contributors</h2>

<table class='contributors-table'>
{% assign contributors = site.github.contributors | sort: 'login' %}
{% for c in contributors %}
<tr>
    <td class='avatar'><img alt='{{c.login}}' src='{{c.avatar_url}}'/></td>
    <td class='name'><code><a href='{{c.html_url}}'>{{c.login}}</a></code></td>
</tr>
{% endfor %}
</table>
