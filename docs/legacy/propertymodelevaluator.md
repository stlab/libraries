---
title: PropertyModelEvaluator
layout: page
tags: [legacy]
comments: true
# redirect_from: /legacy/index.php/
---
<pre>
Usage: pmeval [option]... [file]...
Command line only options:
  --help                produce help message
  -v [ --version ]      print version string


Grammar:
translation_unit        = [ adam_test ].
adam_test               = sheet_specifier [ interaction_list ].
interaction_list        = interaction [ interaction_list ].
interaction             = update_decl | reinitialize_decl
                                      | dump_decl | check_decl | print_decl | assert_decl
                                      | contributing_decl | trailing_comment | .
update_decl             = "update" dictionary.
reinitialize_decl       = "reinitialize" dictionary.
dump_decl               = "dump".
check_decl              = "check" dictionary.
print_decl              = "print" "(" expression ")".
assert_decl             = "assert" "(" expression ")".
contributing_decl       = "contributing".

keywords                += "update" | "reinitialize" | "check" | "dump" | "print"
                           | "assert" | "contributing" .
<pre/>
