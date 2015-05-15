# stlab/libraries

ASL libraries will be migrated here in the stlab namespace, new libraries will be created here.

[ This is temporary documentation - to be replaced at a later date. ]

## <stlab/future>

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
```
