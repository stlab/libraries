---
layout: page
title: About
tab: About
permalink: /about/
---

stlab is the ongoing work of what was Adobe's Software Technology Lab. The Adobe Source Libraries (ASL), Platform Libraries, and new stlab libraries are hosted on [github](https://github.com/stlab).

<h2>Releases</h2>

<table class='definition-table'>
<tr class='headers'>
    <th>Release</th>
    <th>Description</th>
    <th>Download Source</th>
</tr>
{% assign releases = site.data.releases | sort: 'published_at' | reverse %}
{% for r in releases %}
<tr>
    <td><code><a href='{{r.html_url}}'>{{r.tag_name}}</a></code></td>
    <td>{{r.body | markdownify}}</td>
    <td><a href='{{r.zipball_url}}'>zip</a>&nbsp;|&nbsp;<a href='{{r.tarball_url}}'>tar</a></td>
</tr>
{% endfor %}
</table>

<h2>Contributors</h2>

<table class='contributors-table'>
{% assign contributors = site.data.contributors | sort: 'login' %}
{% tablerow c in contributors cols:5 %}
    <a href='{{c.html_url}}'>
      <center>
        <img alt='{{c.login}}' src='{{c.avatar_url}}'/>
        <br/>
        <code>{{c.login}}</code>
      </center>
    </a>
{% endtablerow %}
</table>
