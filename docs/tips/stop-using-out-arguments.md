---
title: Stop using _out_ arguments
layout: page
tags: [tips]
comments: true
---

## The Problem with Out Arguments

I have been lecturing about [Return-Value-Optimization](http://en.cppreference.com/w/cpp/language/copy_elision) for over a decade, yet I still see a lot of code written in this form:

```cpp
string x;
obj.get(x);
my_class y(x);
```

Where `string` is any non-basic type, `get()` is some function with an out argument, and `my_class::my_class()` is a constructor for some class type.

```cpp
void get(string& out) const;
...
my_class::my_class(const string& in);
```

There are several issues with the above code. Let's improve `get()` first, then we'll move on to `my_class`.

With an out parameter, the implementation of `get()` could take one of several forms: copying some existing data (such as a data member), calculating a new value and assigning it, or building up `out` to be the new value. Examples:

```cpp
void get(string& out) const { out = _member; }
void get(string& out) const { out = string("Calculated result"); }
void get(string& out) const {
    out.clear(); // Don't forget to clear, this is an out, not in/out argument!
    out += "Built up result";
}
```

The problems with these examples are legion:

1. ***Error Prone:*** The comment in the last example points out why this is an error prone pattern. Unless you are careful, you can accidentally rely on the prior state of the object making this an in-out argument.

2. ***Inefficient:*** The compiler is required to copy the data from the source (`obj`) into the destination (`x`). This leads to expenses that can otherwise be avoided.

3. ***Ambiguous:*** The caller has no way to know if the call is in/out and it is their responsibility to clear the value before the call. This ambiguity leaves us open to errors on the calling side.

4. ***Unnecessarily Verbose:*** The code speaks for itself here: there is a lot being said to accomplish very little. This puts an undue burden on the developer, both in terms of understanding and maintaining the code.

## Rewriting Out Arguments

Let us look at how we would write these same three `get()` examples with a return value:

```cpp
const string& get() const { return _member; }
string get() const { return string("Calculated result"); }
string get() const {
	string result;
	result += "Built up result";
	return result;
}
```

This version of the code is better in every way:

1. ***Error Free:*** Because the caller does not have to pass in objects to receive the values, there are no preconditions required.

2. ***Efficient:*** There is a mistaken assumption that this piece of code is less efficient, introducing unnecessary copies. However, that has not been true for a very long time.

  In the first case, we avoid an unnecessary copy: if the client is only reading from the value, no copy takes place. (If they do need a copy, then the code is no worse than it was before.)

  In the second example, the compiler will use Return Value Optimization, which works because the result of a function is actually in the caller scope. The third example uses Named Return Value Optimization (NRVO), which is a specific sub-case of RVO. So in both cases the string is constructed in place and no copy occurs.

  RVO has been implemented in optimized builds for every compiler I've tested for over a decade. It will be required with C++17, so a compliant compiler must perform the optimization always. As of C++11, returning from a function is defined as a `move` operation, so even if the copy isn't elided, so long as the type has a move constructor, no copy occurs.

3. ***Unambiguous:*** This code also avoids the ambiguity as to if the argument is in/out, both for the implementor and the caller. What is getting modified is clear, and we are constructing only one object.

4. ***Terse:*** The code is clearly more compact than its predecessor.

Applying what we have just learned, we can clean up our original example:

```cpp
string x = obj.get();
my_class y(x);
```

We can even write the code without the temporary, and it will be no less performant:

```cpp
my_class y(obj.get());
```

This code is concise and no worse in performance that our original code.

## Improving the In Argument

We aren't looking for "no worse", we want better. So let's look at `my_class::my_class()`. The `in` argument is what is known as a _sink_ argument. A sink argument is one that is stored by, or returned from, the function. Constructors are a common source of sink arguments as the argument is often used to initialize a member. Consider a very common implementation form:

```cpp
my_class::my_class(const string& in) : _member(in) { }
```

This code is making an explicit copy of the string to store it. However, if the argument is a temporary, there is no need to copy the data into place, it can moved into place. To do that, pass the sink argument by value and move it into place:

```cpp
my_class::my_class(string x) : _member(std::move(x)) { }
```

In our calling example code, if `get()` is returning a `const string&` then the value will be copied at the call site and moved into place, but if `get()` is returning the `string` by value, then no copy is made, it is constructed directly in the argument slot, and then moved into place. Even if you were writing code in C++03, before `std::move()`, you could swap the value into place and still avoid the copy:

```cpp
my_class::my_class(string x) { swap(_member, x); }
```

## Error codes

A common usage of out params is to allow for an error state on return.  For example:

```cpp
bool get_value(std::string& out) {
    if(!_can_fetch_value) {
        return false;
    }
    out = fetch_value();
    return true;
}
```

With everything we have learned above we could simply treat the empty state of an object as our invalid case. Assuming `fetch_value()` doesn't throw.

```cpp
std::string get_value() {
    if(!_can_fetch_value) {
        return std::string();
    }
    return fetch_value();
}
```

Using the empty state for objects is prefered as it removes the need for null checks at the call site or accidently letting it flow to unrelated parts of your program.

This above works, but what if `std::string` has no default constructor, or is expensive to create?  How can we avoid creating the object if we don't need to?
Assume the following is how `std::string` is implemented:

```cpp
string() = delete;
string(const char * initial_value) {
    _buffer = malloc(1024 * 1024); // we don't know how much room a user could use!
}
```

How can we work with the cases in the above example? We obviously can't use the empty constructor and we can't use the other constructor as we don't want to waste that memory.

This is where optional shines. It is shipping as standard in C++17 but is also available as `boost::optional`.


```cpp
std::optional<std::string> get_value() {
    if(!_can_fetch_value) {
        return {};
    }
    return fetch_value();
}
```

Here we have avoided creating a string, and and use a very lightweight class instance in the error case. This may look strange at first but it allows for much more expressiveness in the function signature. The signature makes it clear that this operation could possibly fail and the caller has to be aware of it. It can also reduce the amount of branching at the call site (see default case below) while eliminating the need to come up with out-of-band values.

For the default case:
```cpp
auto value = get_value().value_or("not found");
```

## Tricky default values

Some cases make it hard to know what to return to indicate failure. Take the following example

```cpp
int get_user_int(std::string user_input) {
    //parse user input and return value
    return -1; //invalid
}
```

What if the user had wanted `-1` as their value? How would we deal with this? We could throw an exception if we couldn't parse the input.
This means that you have to document this exception, and the user has to remember to catch it.

We could use an out variable to indicate success? Hopefully, you have seen why you should reconsider this.

This is another case where optional shines.

```cpp
std::optional<int> get_user_int(std::string user_input) {
    //parse user input and return value
    return {}; //invalid
}
```
It is now obvious what the error case looks like and there is no ambiguity, no need to throw an exception for a common path, and can avoid allocations. No more using pointers and nullptr checks to indicate/check for an empty state.

If you want to carry some sort of information about why the parsing failed, I encourage the reader to look into `std::variant`.

## In-Out Arguments

What if you _want_ an in-out argument? For example:

```cpp
void append(string& in_out) { in_out += "appended data"; }
```

In this case we can combine the two approaches:

```cpp
string append(string sink) { sink += "appended data"; return sink; }
```

To see how this improves the code, consider a simple example like:

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

### When to Modify In-situ

There are situations where an object has a large local part, so move is expensive. There are also cases where you might only be operating on part of the object (such as sorting a subrange of a vector). In such cases it makes sense to modify the object in place. However, these should be treated as the exception, not the rule.

## Multiple Out Arguments

One reason for using out arguments has been to return multiple items. This case is not quite as clear cut, but I will argue that even here you should return via the function result for most cases.

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

This has most of the same advantages as single argument outputs. Unfortunately, the syntax is a bit more cumbersome because of the requirement to use `tie()`. However, in C++17 we will have [structured bindings](https://skebanga.github.io/structured-bindings/) so the invocation will look like:

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

Without ever copying a string!

`apply()` is available today on Mac in `<experimental/tuple>`.

### Naming Multiple Results

One issue with returning multiple results via a tuple is that it may not be clear from the caller which result is which. Arguments have the same issue at the call site, but you can at least look at the function signature. In the `root_extension()` example the name of the function was chosen to make it clear that the first result is the root, and the second the extension.

Another option is to return a struct.

```cpp
struct root_extension_t {
    string root;
    string extension;
};
```

Unfortunately there is no good way to make this work with `tie()`, you would also need to provide a templated `get` accessor returning a `tuple_element`, this is non-trivial.

With C++17, this does work with structured bindings.

```cpp
root_extension_t root_extension(const string& in) {
	auto p = in.find_last_of('.');
	if (p == string::npos) p = in.size();
	return { in.substr(0, p), in.substr(p, in.size() - p) };
}
auto [root, ext] = root_extension("path/file.jpg");
```

Unfortunately, anonymous structs are not allowed:

```cpp
auto root_extension(const string& in) -> struct { string root; string extension; }; // ERROR
```

The recommendation is that unless you already have a correct type for the result type, and you are using C++17, then use the type as the result. Otherwise return a tuple and use a good name for the function to make the result clear.

## Final thoughts

As with all goals, if you find a case where using an out argument is better than using the function result, please do (and start a discussion here on the topic) but in most use cases I find that out arguments lose by all measures compared to using function results.
