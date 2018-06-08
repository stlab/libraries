---
title: Locale-savvy isspace
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Locale-savvy_isspace
---

isspace() relies on the locale() - we currently document the CEL parser as assuming the "C" locale - we should note the same requirement in the XML parser. Later it would be good to have a library providing this functionality which is wired to utf encoding - most likely this would be along the lines of the current parser syntax - something like the following:

```cpp
namespace implementation {

template<std::size_t S>
struct is_space;

template<>
struct is_space<1>
{
	template<typename I>
	bool operator()(I& x) const
	{
		if (space_table[*x]) { ++x; return true; }
		return false;
	}
};

template<>
struct is_space<2>
//...

} // namespace implementation

template<typename I> // I models InputIterator where value_type(I) == UTF encoded character
bool adobe::is_space(I& x)
{
	return adobe::implementation::is_space<sizeof(std::iterator_traits<I>::value_type)>()(x)
}
```
