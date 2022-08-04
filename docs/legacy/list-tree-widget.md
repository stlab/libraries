---
title: List/Tree Widget
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/List/Tree_Widget
---

_(For the time being the focus will be on lists, not heirarchical trees.)_

The tricky part of a list controller/view is that you have a controller/view on a sequence. You need to figure out:
1. How to communicate requests for changes to the sequence
2. What those requests are
3. How to communicate a change in the visible portion of the sequence to the view

We assume the sequences in a list are disjoint.

#### Contents
{:.no_toc}
1. contents
{:toc}

### Related Docs

[Win32 Tree View Control Documentation](https://msdn.microsoft.com/en-us/library/windows/desktop/bb759988(v=vs.85).aspx)

[Win32 List View Control Documentation](https://msdn.microsoft.com/en-us/library/windows/desktop/bb774737(v=vs.85).aspx)

[Carbon Tree Control API Documentation (Data Browser)](http://mirror.informatimago.com/next/developer.apple.com/documentation/Carbon/Conceptual/display_databrowser/index.html)

## API

_This API set will be updated piecemeal as I am able to further the implementation_

Assume for the time being that the model_type is a simple string for now...

### Model

```cpp
  template <typename T>
  class sequence_model
  {
  public:
      sequence_model(std::size_t count = 0);

      typedef T                                          value_type;
      typedef dirty_value<T>                             dirty_type;
      typedef std::vector<dirty_type>                    model_type;
      typedef typename model_type::iterator              iterator;
      typedef typename model_type::const_iterator        const_iterator;
      typedef boost::signals::connection                 connection_type;
      typedef typename model_type::size_type             size_type;
      typedef boost::function<void (iterator, iterator)> inplace_operation_proc_t;

      typedef boost::function<void (size_type pos, const T* first, const T* last)> monitor_insert_proc_t;
      typedef boost::function<void (size_type index, const T& value)>              monitor_set_proc_t;

      static const size_type npos;

npos is used as an out-of-bounds index, much like std::string::npos.

      const_iterator begin() const;
      const_iterator end() const;
```

These are const because the sequence is not modifiable via iterators outside of an inplace operation.

```cpp
      size_type size() const;
      size_type max_size() const;

      bool empty() const;

      const T& at(size_type n) const;
      const T& operator[](size_type n);
```

These are const because the sequence is not modifiable via iterators outside of an inplace operation.

```cpp
      void push_back(const value_type& x);

      void set(size_type pos, const value_type& x);
```

Allows the setting of a single position's value.

```cpp
      template <typename I>
      void insert(size_type pos, I first, I last);
```

Allows the insertion of a range of values.

```cpp
      void inplace_operation(const inplace_operation_proc_t& proc,
                             size_type                       pos = 0,
                             size_type                       n = sequence_model::npos);
```

given an index, a count, and an inplace operation, this function will allow for a mutable transaction to take place within the sequence. The inplace_operation_proc is given mutable iterators based on the requested position and count variables. When the operation completes, the sequence_model will iterate over the range and check to see which values (if any) changed, and make proper notification of these changes to the attached views.

```cpp
      connection_type monitor_insert(const monitor_insert_proc_t& proc);
      connection_type monitor_set(const monitor_set_proc_t& proc);
```

Allows for clients to register one or more views into the sequence model.

```cpp
  };
```

#### Notes on the Sequence Model

dirty_type is an implementation-defined method of discovering what values have changed due to an inplace operation; the client of the sequence_model need not be concerned with it.

### View

```cpp
  struct list_view_t
  {
    typedef model_type::value_type value_type;
    typedef model_type::size_type  size_type;

    void set(size_type index, const value_type& value);

    void insert(size_type pos, const value_type* first, const value_type* last);
  };
```

### Controller

```cpp
  struct list_controller_t
  {
    typedef model_type::inplace_operation_proc_t inplace_operation_proc_t;
    typedef boost::function<void (size_type, const value_type&)> monitor_set_callback_t;
    typedef boost::function<void (size_type, const value_type*, const value_type*)> monitor_insert_callback_t;
    typedef boost::function<void (const inplace_operation_proc_t& proc, size_type, size_type)> monitor_inplace_operation_callback_t;

    void enable(bool make_enabled);

    void monitor_set(const monitor_set_callback_t& how);

    void monitor_insert(const monitor_insert_callback_t& how);

    void monitor_inplace_operation(const monitor_inplace_operation_callback_t& how);
  };
```
