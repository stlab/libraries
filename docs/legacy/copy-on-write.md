---
title: Copy On Write
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Copy_On_Write
---
### Reference Links

* [Current ASL Documentation](https://stlab.adobe.com/classadobe_1_1copy__on__write.html)
* [Regular Type Documentation](https://stlab.adobe.com/group__concept__regular__type.html)

### Additional Documentation

Any type that models a `RegularType` can be wrapped to use copy_on_write. To use copy_on_write you specify your variable as such:

<pre>
// to be used later in the example
void stream_out(const int& x)
{
    std::cout << x << ' ';
}

typedef adobe::copy_on_write<std::vector<int> > cow_vec_t;

cow_vec_t vec1;
</pre>

Once you have your type established, you reference the wrapped type either using <code>operator -></code> (which returns a <code>const reference</code> to your type) or via the <code>.write()</code> mechanism, which returns a writeable reference. It is at the time when you call <code>write()</code> that the copy on write wrapper spins off a copy of the encapsulated element if there are more than one references to the shared element under the hood

<pre>
vec1.write().push_back(42);
vec1.write().push_back(100);
vec1.write().push_back(1978);

std::vector<int>::const_iterator first(vec1->begin());
std::vector<int>::const_iterator last(vec1->end());

// ... do something with first, last ...
</pre>

Assignment doesn't spin off a new copy of the thing, it just increments the reference count:

<pre>
cow_vec_t vec2;

vec2 = vec1; // does not spin off a copy
</pre>

However, using <code>write()</code> a copy is spun off if need be:

<pre>
vec2.write() /*now the copy is spun off*/ .push_back(84);

adobe::for_each(vec1.get(), stream_out); // prints "42 100 1978 "
adobe::for_each(vec2.get(), stream_out); // prints "42 100 1978 84 "
</pre>

I say "if need be" because if there is only one reference to the current object then the write operation returns the current element itself. You can check if the reference you are currently holding is the only reference via <code>unique</code>:

<pre>
cow_vec_t vec3;

vec3.write().push_back(42);

assert(vec3.unique_instance());   // ... only one instance refers to the shared part
</pre>

You can also check to see if two copy_on_write instances are pointing to the same shared reference:

<pre>
cow_vec_t vec4;

vec4.write().push_back(42);

cow_vec_t vec4(vec5);

assert(vec4.identity(vec5));
</pre>
