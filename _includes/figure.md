<p style='text-align: center;' markdown='1'>

{% assign figure-number = figure-number | default: 0 | plus: 1 %}
{% assign figure-index = figure-number | minus: 1 %}

{% if page.chapter %}
{% assign -figure-decimal = page.chapter | append: '.' | append: figure-number %}
{% else %}
{% assign -figure-decimal = figure-number %}
{% endif %}

{% assign -figure-ref = '[figure ' | append: -figure-decimal | append: '](#' | append: include.name | append: ')' %}

{% if figure-reference %}
    {% assign figure-reference = ((figure-reference | join: '$') | append: '$' | append: -figure-ref) | split: '$' %}
{% else %}
    {% assign figure-reference = -figure-ref | split: '$'  %}
{% endif %}

{::comment}Caption is not included in alt-text because it contains markup{:/comment}
![figure {{-figure-decimal}}][{{include.name}}]\\

figure {{-figure-decimal}}: {{include.caption}}
</p>{: #{{include.name}} }

[{{include.name}}]: figures/{{include.name}}
