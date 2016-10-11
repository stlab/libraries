---
title: Tips
tagline: Goals for Software Developers
layout: page
---

## Contents

<dl class='posts'>
{% for p in site.pages %}
    {% if p.tags contains 'tips' and p.url != page.url %}
        <dt><a href="{{ BASE_PATH }}{{ p.url }}">{{ p.title }}</a></dt>
    {% endif %}
{% endfor %}
</dl>

