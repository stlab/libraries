---
title: How To Write A Simple Lexical Analyzer or Parser
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/How_To_Write_A_Simple_Lexical_Analyzer_or_Parser
---
## Introduction

This _paper_ is a response to an e-mail question asking if anyone had code to validate that a sequence of characters was a valid XML name which was not a reserved XML name (starting with <tt>'x'|'X' 'm'|'M' 'l'|'L'</tt>) or a qualified name (one containing <tt>':'</tt>). [_See Section 2.3 of the XML Specification_](http://www.w3.org/TR/REC-xml/#sec-common-syn).

Languages are defined using a grammar. There are many notations for grammars but most use some variant of [Extended Backus-Naur form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) (EBNF), XML is no exception to this rule. The variant of EBNF used for the XML notation is described in [Section 6 of the XML Specification](http://www.w3.org/TR/REC-xml/#sec-notation). Transforming an EBNF grammar into a lexical analyzer and/or parser is a relatively straight forward process. For simplicity I define the lexical analyzer as the system which transforms a stream of characters into tokens and the parser as the system which transforms a stream of tokens into a sequence of actions for each production rule. There are libraries, such as the [Boost Spirit Library](https://www.boost.org/doc/libs/1_35_0/libs/spirit/index.html) and tools such as [Lex](https://en.wikipedia.org/wiki/Lex_(software)) and [Yacc](https://en.wikipedia.org/wiki/Yacc) which can be used to aid in writing lexical analyzers and parser. Knowing how to write these systems simply and directly is an invaluable tool in any programmers tool chest and experience with writing such systems will make you more productive when using libraries and tools and give you some insight into when you should and should not use them. Some issues (including this one) can also be addressed by writing a regular expression using a library such as the [Boost Regex Library](https://www.boost.org/doc/libs/1_35_0/libs/regex/doc/html/index.html) - however, dealing with the numerous ranges of values in the grammar production with this problem are difficult to express in a regular expression. A [finite automata](https://en.wikipedia.org/wiki/Finite-state_machine) can also be constructed for many such problems (including this one) and such systems have the advantage of inverting control flow - you put values into a finite automata as opposed to executing a parse function which pulls values from a stream.

When you look at a typical EBNF grammar it will frequently not distinguish which parts are the lexical analyzer production rules and which are the parser production rules - the structure of both a simple parser and simple lexer will be the same. The separation is the point where the value returned by the production rule is not a character from the stream, but some form of token. Because the structure is the same I'm going to refer from here on to both parts as simply the _parser_.

## A Simple Recursive Descent Parser

To solve the problem of validating an XML name we're going to write a simple recursive descent, or top-down, parser. The idea is to read from a stream of characters denoted by a pair of InputIterators - we will only need to read each character once without backtracking. A grammar which can be read in this form without backtracking is known as an [LL(1)](http://en.wikipedia.org/wiki/LL_parser) grammar.

To do this we are going to translate each production rule into a function which will return a <tt>bool</tt> with the value <tt>true</tt> if it recognizes the production and false if it does not. If it does recognize the production then the stream will be advanced to the end of the production and a value will be updated with the recognized production. Otherwise, the stream and value are left unchanged. Such a function will follow this pattern:

<pre>
bool is_production(I& first, I last, T& value);
</pre>

The stream denoted by <tt>[first, last)</tt> is going to be UTF16 code points because that is how the XML specification is defined. We'll define code_point_t to be a 16 bit integer:

<pre>
typedef uint16_t code_point_t;
</pre>

Rather than passing <tt>first</tt> and <tt>last</tt> through all of our functions we're going to create a simple struct which holds them and write the <tt>is_production()</tt> as a member function:

```cpp
template <typename I> // I models InputIterator
// require value_type(I) == code_point_t
struct parser
{
    bool is_production(T& value);

    I f_m; // first
    I l_m; // last
};
```

Writing the body of the production is simple. We start at the top level production rule and work our way down - in this case we are interested in parsing <tt>Name</tt>. The <tt>Name</tt> production rule looks like this:

```
Name ::= (Letter | '_' | ':') (NameChar)*
```

And we could write this production something like the following:

```cpp
bool is_name(string16_t& s)
{
    string16_t   result;
    code_point_t c;

    /* (Letter | '_' | ':') */
    if (is_letter(c) || is_match('_', c) || is_match(':', c)) ; else return false;

    result += c;

    /* (NameChar)* */
    while(is_name_char(c)) result += c;

    s = result;
    return c;
}
```

Here string16_t is our token type. For this problem, we don't actually care what the token value is - and constructing the token is relatively expensive. If we were writing a full-blown XML parser we would spend some energy on dealing with tokens efficiently. This code also doesn't tell is if the name is qualified or reserved - those are attributes of our token value. We rewrite the function to drop the token and add some attributes. We'll deal with attributes in a similar fashion to the value of the production rule, except if the production rule is recognized the attributes will be accumulated (as opposed to overwritten). We don't need to pass the attribute to all production rules, only those that have attributes. Here is the definition of our attribute type:

```cpp
enum xml_parse_attribute_t {
    xml_none        = 0,
    xml_qualified   = 1,
    xml_reserved    = 2
};

xml_parse_attribute_t& operator|=(xml_parse_attribute_t& r, xml_parse_attribute_t x)
{ r = xml_parse_attribute_t(r | x); return r; }
```

We define <tt>operator|=()</tt> on our type to make it simple to accumulate. <tt>xml_qualified</tt> will mean that the name contains a <tt>':'</tt> and <tt>xml_reserved</tt> will mean it starts with the sequence <tt>xml</tt>. To check if the name is reserved we unroll the first couple of iterations of our loop:

```cpp
bool is_name(xml_parse_attribute_t& a)
{
    code_point_t c;

    if (is_match(':', c)) { a |= xml_qualified; }
    else if (is_letter(c) || is_match('_', c)) ; else return false;
    bool reserved = c == code_point_t('X') || c == code_point_t('x');

    if (is_name_char(c, a)) ; else return true;
    reserved &= (c == code_point_t('M') || c == code_point_t('m'));

    if (is_name_char(c, a)) ; else return true;
    reserved &= (c == code_point_t('L') || c == code_point_t('l'));

    while (is_name_char(c, a)) ;

    if (reserved) a |= xml_reserved;

    return true;
}
```

Now we can write our remaining production rules. We'll tackle <tt>NameChar</tt> next. This is the only other production rule which needs to deal with attributes because it may contain a <tt>':'</tt>.

```
NameChar ::= Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
```

The specification of the XML parser is in terms of UCS2 code points (as opposed to characters) and this production is only going to recognize a single code point. Except for the minor complexity introduced to handle the attribute, the code is nearly identical to the production rule.

```cpp
bool is_name_char(code_point_t& c, xml_parse_attribute_t& a)
{
    if (is_match(':', c)) { a |= xml_qualified; return true; }

    return is_letter(c) || is_digit(c) || is_match('.', c)
        || is_match('-', c) || is_match('_', c) || is_combining_char(c) || is_extender(c);
}
```

The <tt>is_letter()</tt> function is even simpler:

```
Letter ::= BaseChar | Ideographic
```

```cpp
bool is_letter(code_point_t& c) { return is_base_char(c) || is_ideographic(c); }
```

The <tt>is_match()</tt> is a helper function for _terminal_ productions. A terminal production is a production which is not defined in terms of another production. It is in the terminal productions where the interesting work is done. Here is the body.

```cpp
bool is_match(code_point_t x, code_point_t& c)
{
    if (f_m == l_m || *f_m != x) return false;
    c = x; ++f_m; return true;
}
```

If we are at the end of our steam or the current character is not a match then return <tt>false</tt>. Otherwise we set our value to the matched character, increment the stream, and return <tt>true</tt>.

The remaining productions are all of a form which match one of a set of code points:

```
Ideographic ::= [#x4E00-#x9FA5] | #x3007 | [#x3021-#x3029]
```

Some of the tables, such as the one for <tt>BaseChar</tt> are quite large making it prohibitively expensive to write an expression like <tt>(0x4E00U <= x && x <= 0x9FA5U) || (x == 0x3007U) || ...</tt>. Instead we will create a simple lookup table. When dealing with a parser that works with a simple <tt>char</tt> type we could just build a table with 256 elements that contained an enumeration indicating what production rule the character matched. A table of 16 bit values would be a bit large (but we could do it - 64K elements) - but instead we'll create an array of values denoting ranges which contain the code points we want to match and search to find if our character is in one of the ranges. To do that we'll create a semi-open ranges for each character range of the form <tt>(f0, l0], (f1, l1], ...</tt> and then use <tt>lower_bound()</tt> to search the table, if we end up on an odd index (starting at 0) then the item is in the range, an even index it is not. This strategy means we can't represent a range which includes <tt>0</tt> but <tt>0</tt> is not a valid character in an XML production. The code for the table lookup for <tt>Ideographic</tt> is this:

```cpp
bool in_ideographic(code_point_t c)
{
    static const code_point_t table[] =
    {
        0x4DFFU, 0x9FA5U, // [#x4E00-#x9FA5]
        0x3006U, 0x3007U, // #x3007
        0x3020U, 0x3029U  // [#x3021-#x3029]
    };

    return (adobe::lower_bound(table, c) - boost::begin(table)) & 0x01;
}
```

To transform from the closed ranges to the semi-open ranges we subtract one from the first item in the range. This code doesn't have to be a member function and is not a template - we can put these lookup tables in a .cpp file. We'll use this function in our <tt>is_ideographic()</tt> member function:

```cpp
bool is_ideographic(code_point_t& c) { return is_match(in_ideographic, c); }
```

<tt>is_match()</tt> is another variant of our previous is_match which takes a predicate function instead of a value:

```cpp
bool is_match(bool (*p)(code_point_t), code_point_t& c)
{
    if (f_m == l_m || !p(*f_m)) return false;
    c = *f_m; ++f_m; return true;
}
```

The remaining productions are written the same way - just with larger tables. Our parser is complete!

Finally - to make this a little simpler to invoke we write a wrapper function which just creates a temporary instance of the class and we add one last check to make sure that the valid name made up the entire range provided:

```cpp
template <typename I> // I models InputIterator
// require value_type(I) == code_point_t
bool valid_name_unqualified_unreserved(I f, I l)
{
    xml_parser<I> parser = { f, l };
    xml_parse_attribute_t a = xml_none;
    return parser.is_name(a) && parser.f_m == l && a == xml_none;
}
```

That's the complete code. Besides having a solution to our problem we have a general framework for building other solutions to parsing problems and a pretty descent start that can be reused for other XML related issues. Parsers written in this style are simple to extend even if they are packaged as a library, you can simply inherit from the struct and add additional productions. The code described here took very little time to write (the biggest hassle was building the larger lookup tables from the productions in the XML specification.

The complete example file can be found [here](figures/xml_parser_example.cpp).
