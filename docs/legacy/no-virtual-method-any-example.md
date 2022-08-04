---
title: No virtual method "any" example
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/No_virtual_method_any_example
---
```cpp
struct concept_t;

struct vtable_t {
    void (*destruct)(const concept_t&);
    concept_t* (*copy)(const concept_t&);
    void (*print)(const concept_t&);
};

struct concept_t {
    explicit concept_t(const vtable_t& x) : vtable_(&x) { }

    void destruct() const { vtable_->destruct(*this); }
    concept_t* copy() const { return vtable_->copy(*this); }
    void print() const { vtable_->print(*this); }

    const vtable_t* vtable_;
};

template <typename T>
struct model : concept_t {
    explicit model(T& x) : concept_t(vtable_), data_(move(x)) { }
    static void destruct(const concept_t& x) { static_cast<const model&>(x).~model(); }
    static concept_t* copy(const concept_t& x) { return new model(static_cast<const model&>(x)); }
    static void print(const concept_t& x) { cout << static_cast<const model&>(x).data_ << endl; }

    static const vtable_t vtable_;

    T data_;
};

template <typename T>
const vtable_t model<T>::vtable_ = {
    &model::destruct,
    &model::copy,
    &model::print
};

class any {
public:
    template <typename T>
    any(T x) : object_(new model<T>(x)) { }
    ~any() { object_->destruct(); operator delete(static_cast<void*>(object_)); }
    any(const any& x) : object_(x.object_->copy()) { }

    friend inline void swap(any& x, any& y) { std::swap(x.object_, y.object_); }
    any& operator=(const any& x) { any tmp(x); swap(*this, tmp); return *this; }

    friend inline void print(const any& x) { x.object_->print(); }
private:
    concept_t* object_;
};
```
