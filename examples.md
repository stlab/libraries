---
layout: page
title: Library Examples
tab: Examples
permalink: /examples/
---

The following is a list of published examples and the part of the stlab they demonstrate:

<table class='definition-table'>
{% for p in site.pages %}
  {% unless p.example %}
    {% continue %}
  {% endunless %}
  <tr>
      <td>
      <a href='{{p.url}}'><code>{{p.title}}</code></a>
      </td>
      <td>
        {% for e in p.example %}
          <a href='{{p.url | split: '/' | pop | push: e | join: '/'}}'><code>{{e}}</code></a><br/>
        {% endfor %}
      </td>
  </tr>
{% endfor %}
</table>
