# stlab/libraries

ASL libraries will be migrated here in the stlab namespace, new libraries will be created here.

[ This is temporary documentation - to be replaced at a later date. ]

## <stlab/future>

This is a proof of concept implementation of a packaged task and future to replace the standard components. This is a list of some of the differences from standard (as of C++14) and boost (as of boost 1.58.0).

There is no promise type, packaged_task and futures are created as pairs with the package() function.

package_task is copyable, not just movable, and the call operator is const. This allows packaged_task objects to be
stored in std::function<>. Once any copy of a packaged_task<> is called, all copies are invalid.

future is also copyable, there is no need for a shared future. If a future is only used as an rvalue and there are no copies then the value returned, by get_try or through a continuation, will be moved.

Multiple continutations may be attached to a single future with then(). then() is declared const since it does not mutate the result object of the future.

The continuation is called with the value type, not the future. A sink argument to a continuation should take the argument by value and move the object as needed. If the continuation reads the argument it should take it by const&. Behavior of modifying the argument through a non-const reference is undefined (may be a compliation error).

If the last copy of a future destructs, the associated task and any held futures for the task arguments are released and the associated packaged_task will become a no-op if called.

There are no wait() or get() function. Instead there is a get_try() which returns an optional<T> (or if T is void, the result is a bool with true indicating the associated task has executed.

If the associated task through an exception, get_try() with rethrow the exception.

[ TODO - for notification of errors the plan is to add a recover() clause to futures which is passed the exception and may return a value T or rethrow. recover() will be executed prior to continuations. ]

when_all() takes an n'ary function and n futures as arguments.

[ TODO - although futures currently work with limitations on move only types (you can only have one continuation and
one call to get) I should enable_if future itself so that it T is move only then the future is move only. ]

```c++
template<typename R, typename ...Args >
class packaged_task<R (Args...)> {
  public:
    packaged_task();
    ~packaged_task();

    packaged_task(const packaged_task&);
    packaged_task(packaged_task&&) noexcept;
    packaged_task& operator=(const packaged_task&);
    packaged_task& operator=(packaged_task&&) noexcept;

    template <typename... A>
    void operator()(A&&... args) const;
};

template <typename T>
class future {
  public:
    future();
    ~future();

    future(const future&);
    future(future&&) noexcept;
    future& operator=(const future&);
    future& operator=(future&& x) noexcept;

    template <typename F>
    auto then(F&& f) const&; // -> future<result_of_t<F(T)>>

    template <typename S, typename F>
    auto then(S&& s, F&& f) const&; // -> future<result_of_t<F(T)>>

    template <typename F>
    auto then(F&& f) &&; // -> future<result_of_t<F(T)>>

    template <typename S, typename F>
    auto then(S&& s, F&& f) &&; // -> future<result_of_t<F(T)>>

    bool cancel_try();

    auto get_try() const&; // -> T == void ? bool : optional<T>
    auto get_try() &&; // -> T == void ? bool : optional<T>
};

template <typename S, typename F, typename... Ts>
auto when_all(S, F, future<Ts>... args); // -> future<result_of_t<F(Ts...>>

template <typename Sig, typename S, typename F>
auto package(S s, F f); // -> pair<packaged_task<Sig>, future<result_of_t<Sig>>>;

template <typename S, typename F, typename ...Args>
auto async(S s, F&& f, Args&&... args) -> future<std::result_of_t<F (Args...)>>
```
