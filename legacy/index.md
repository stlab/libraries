---
title: Legacy
layout: page
---

These pages have been migrated from stlab.adobe.com/wiki for preservation. They may eventually be revised and moved into a more appropriate location on the site.

## Contents

<dl class='posts'>
{% for p in site.pages %}
    {% if p.tags contains 'legacy' and p.url != page.url %}
        <dt><a href="{{ BASE_PATH }}{{ p.url }}">{{ p.title | markdownify }}</a></dt>
    {% endif %}
{% endfor %}
</dl>

