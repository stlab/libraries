---
title: Stop using _out_ arguments
layout: page
tags: [tips]
comments: true
---
## Out Arguments

Despite lecturing about Return-Value-Optimization for over a decade, I still see a lot of code of the form:

```cpp
string x;
obj.get(x);
my_class y(x);
```

Where `string` is any complex type and the signatures of `get()` and `my_class::my_class()` are:

```cpp
void get(string& out) const;
...
my_class::my_class(const string& in);
```

There are several issues with the above code. It is error prone, inefficient, ambiguous, and unnecessarily verbose. Consider the same piece of code written as:

```cpp
my_class y(obj.get());
```
Besides being more compact, there is no ambiguity about what is getting modified, we are constructing only one object.

There is a mistaken assumption that this piece of code is less efficient, introducing unnecessary copies. However, that has not been true for a very long time. To understand why, let's first look at `get()`. With an out parameter, the implementation of `get()` could take one of several forms, copying some existing date, such as a data member, calculating a new value and assigning it to `out`, or building up `out` to be the new value. Examples:

```cpp
void get(string& out) const { out = _member; }
void get(string& out) const { out = string("Calculated result"); }
void get(string& out) const {
    out.clear(); // Don't forget to clear, this is an out, not in/out argument!
    out += "Built up result";
}
```

The comment in the last examples points out why this is an error prone pattern, unless you are careful, you can accidentally rely on the prior state of the object making this an in-out argument. Worse, the caller has no way to know if the call is in/out and it is their responsibility to clear the value before the call.

Now, let us look at how we would write these same three example with a return value:

```cpp
const string& get() const { return _member; }
string get() const { return string("Calculated result"); }
string get() const {
	string result;
	result += "Built up result";
	return result;
}
```

In the first case, we avoid an unnecessary copy, if the client is only reading from the value, they don't need an additional copy.

In the second example, the compiler will use Return Value Optimization, which works because the result of a function is actually in the caller scope. So in both cases the string is constructed in place and no copy occurs. RVO has been implemented in optimized builds for every compiler I've tested for over a decade and it will be required with C++17 so a compliant compiler must perform the optimization always. And as of C++11, returning from a function is defined as a _move_ operation, so even if the copy isn't elided, so long as the type has a move constructor, no copy occurs. This code also avoid the ambiguity as to if the argument is in/out, both for the implementor and the caller.

Just knowing this we can clean up our original example:

```cpp
string x = obj.get();
my_class y(x);
```

And we can even write the code without the temporary and it will be no-worse than the original code for performance:

```cpp
my_class y(obj.get());
```

But we aren't looking for "no-worse" we want better. So let's look at `my_class::my_class()`. The `in` argument is what is known as a _sink_ argument. A sink argument is any argument that is stored by, or returned from, the function. Constructors are a common source of sink arguments as the argument is often used to initialize a member. Consider a very common implementation form:

```cpp
my_class::my_class(const string& in) : _member(in) { }
```

This code is making an explicit copy of the string to store it. However, if the argument is a temporary, there is no need to copy the data into place, it can moved into place. To do that, we pass the sink argument by value and move it into place:

```cpp
my_class::my_class(string x) : _member(std::move(x)) { }
```

In our calling example code, if `get()` is returning a `const string&` then the value will be copied at the call site and moved into place, but if `get()` is returning the `string` by value, then no copy is made, it is constructed directly in the argument slot, and then moved into place. Even if you were writing code in C++03, before `std::move()`, you could swap the value into place and still avoid the copy:

```cpp
my_class::my_class(string x) { swap(_member, x); }
```
## In-Out Arguments

But what if you _wanted_ an in-out argument? For example:

```cpp
void append(string& in_out) { in_out += "appended data"; }
```

In this case we can combine the two approaches:

```cpp
string append(string sink) { sink += "appended data"; return sink; }
```

To how this is improves the code, consider a simple example like:

```cpp
string x("initial value");
append(x);
```

This will become:

```cpp
string x = append("initial value");
```

If you already had the object you wanted to append to, you can still do that:

```cpp
x = append(std::move(x));
```

There are situations where an object has a large local part, so move is expensive, and it makes sense to modify the object in place, however, these should be treated as the exception.

## Multiple Out arguments

One reason for use out arguments has been to return multiple items. This case is not quite as clear cut, but I will argue that even here you should return via the function result for most cases.

Here is an example:

```cpp
void root_extension(const string& in, string& root, string& ext) {
	auto p = in.find_last_of('.');
	if (p == string::npos) p = in.size();
	root = in.substr(0, p);
	ext = in.substr(p, in.size() - p);
}

//...

string root;
string ext;
root_extension("path/file.jpg", root, ext);
```

This can be rewritten as:

```cpp
auto root_extension(const string& in) {
	auto p = in.find_last_of('.');
	if (p == string::npos) p = in.size();
	return make_tuple(in.substr(0, p), in.substr(p, in.size() - p));
}

//...

string root;
string ext;
tie(root, ext) = root_extension("path/file.jpg");
```

`std::tie` and `std::make_tuple` were added in C++11 (though available prior to that as part of Boost and then TR1).

This has most of the same advantages as single argument outputs though the syntax is currently a bit more cumbersome because of the requirement to use `tie()`. However, in C++17 (coming soon) we will have structured bindings so the invocation will look like:

```cpp
auto [root, ext] = root_extension("path/file.jpg");
```

C++17 also includes an `std::apply()` function that can be used to expand a tuple into a set of arguments. So you will be able to write:

```cpp
cout << apply([](string root, const string& ext){
	return move(root) + "-copy" + ext;
}, root_extension("path/file.jpg")) << endl;
```

And this will print:

```
path/file-copy.jpg
```

Without ever copying a string. `apply()` is available today on Mac in `<experimental/tuple>`.

## Final thoughts

As with all goals, if you find case where using an out argument is better than using the function result, please do (and start a discussion here on the topic) but in most use cases I find that out arguments lose by all measures compared to using function results.




