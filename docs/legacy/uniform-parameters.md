---
title: Uniform Parameters
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Uniform_Parameters
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Proposal

Ralph Thomas proposed being able to handle both named and unnamed parameters in the same function call in CEL. I've made one attempt at implementing this, with some interesting ramifications worth discussion.

For the current implementation I modified the CEL syntax to allow named and unnamed parameter in a single parameter list - the modified syntax is:

```ebnf
expression                  = or_expression ["?" expression ":" expression].

or_expression               = and_expression { "||" and_expression }.
and_expression              = equality_expression { "&&" equality_expression }.
equality_expression         = relational_expression { ("==" | "!=") relational_expression }.
relational_expression       = additive_expression { ("<" | ">" | "<=" | ">=") additive_expression }.
additive_expression         = multiplicative_expression { ("+" | "-") multiplicative_expression }.
multiplicative_expression   = unary_expression { ("*" | "/" | "%") unary_expression }.

unary_expression            = postfix_expression | (unary_operator unary_expression).
unary_operator              = "+" | "-" | "!".

postfix_expression          = primary_expression { ("[" expression "]") | ("." identifier) }.

primary_expression          = name | number | boolean | string | "empty" | array | dictionary
                                | variable_or_fuction | ( "(" expression ")" ).

variable_or_function        = identifier ["(" [named_argument_list] ")"].

array                       = "[" [argument_list] "]".
dictionary                  = "{" [named_argument_list] "}".

argument_expression_list    = named_argument_list | argument_list.

argument_list               = expression { "," expression }.
named_argument_list         = named_argument { "," named_argument }.
named_argument              = [identifier ":"] expression.

name                        = "@" identifier.
boolean                     = "true" | "false".
```

The implementation stores the unnamed parameter in an array, in the order they appear, and the array is stored in an unnamed entry in the dictionary.

This "spills" over to allow dictionaries to have unnamed entries. To recover these entries I allowed index a dictionary by integer, indexing by integer will _only_ recover unnamed elements.

```none
{ key: "Value", "No Key" }[0] == "No Key"
{ key: "Value", "No Key" }[@key] == "Value"
```

This is similar to a table in the Lua programming language.

There are several ways I can see to proceed from here:

* This makes arrays somewhat redundent in the CEL syntax, they could be removed, or we could keep them as a form of constrained structure.
* Array parameters could be seperated from dictionaries, and support for unnamed values in dictionaries could be removed. The downfall here is that parameter lists wouldn't be describable with the basic language constructs.
* Keep things as described above (though it just feels like a hack to me).
* Generalize things even more (like allowing arbitrary key types) and turn dictionaries into full blown Lua tables (this also feels like a hack though).

## Discussion

* _Foster Brereton_ -- Why do the following feel like a hack:
    * Keeping the CEL as described above?
    * Turning dictionaries into Lua tables?
