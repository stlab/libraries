# stlab/libraries

ASL libraries will be migrated here in the stlab namespace, new libraries will be created here.

[ This is temporary documentation - to be replaced at a later date. ]

# Build Status: [![Build Status](https://travis-ci.org/FelixPetriconi/libraries.svg?branch=UnitTests)](https://travis-ci.org/FelixPetriconi/libraries)

## <stlab/future>

This is a proof of concept implementation of a packaged task and future to replace the standard components. This is a list of some of the differences from standard (as of C++17) and boost (as of boost 1.60.0):

- There is no promise type, packaged_task and futures are created as pairs with the package() function.

- package_task is copyable, not just movable, and the call operator is const. This allows packaged_task objects to be stored in std::function<>. Once any copy of a packaged_task<> is called, all copies are invalid.

- future is also copyable, there is no need for a shared future. If a future is only used as an rvalue and there are no copies then the value returned, by get_try or through a continuation, will be moved.

- Multiple continutations may be attached to a single future with then(). then() is declared const since it does not mutate the result object of the future.

- The continuation is called with the value type, not the future. A sink argument to a continuation should take the argument by value and move the object as needed. If the continuation reads the argument it should take it by const&. Behavior of modifying the argument through a non-const reference is undefined (may be a compliation error).

- If the last copy of a future destructs, the associated task and any held futures for the task arguments are released and the associated packaged_task will become a no-op if called.

- There are no wait() or get() function. Instead there is a get_try() which returns an optional<T> (or if T is void, the result is a bool with true indicating the associated task has executed).

- If the associated task through an exception, get_try() with rethrow the exception.

- For a future<T> if T is move only then the future is move only and can only contain one continuation.

- In case a recover() clause is defined, then a failed future is passed to it and offers an appropriate error handling. recover() will be executed prior to continuations.

when_all() takes either an n'ary function and n futures as arguments or a variable range of futures

when_any() takes either an n'ary function and n (>=1) futures as argument or a variable range of futures and proceeds when at least one of the futures successds.

Here is a trivial scheduler using threads:

```c++
auto schedule = [](auto f) // F is void() and movable
{
    thread(move(f)).detach();
};
```

Here is an example scheduler function that uses Apple's libdispatch.

```c++
auto schedule = [](auto f) // F is void() and movable
{
    using f_t = decltype(f);

    dispatch_async_f(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
            new f_t(std::move(f)), [](void* f_) {
                auto f = static_cast<f_t*>(f_);
                (*f)();
                delete f;
            });
};
```

Here is an example scheduler that executes the task in the Qt main loop, e.g. to
update an UI element.
```C++
class QtScheduler
{
    class EventReceiver;

    class SchedulerEvent : public QEvent
    {
        std::function<void()> _f;
        std::unique_ptr<EventReceiver> _receiver;

  public:
    explicit SchedulerEvent(std::function<void()> f)
      : QEvent(QEvent::User)
      , _f(std::move(f))
      , _receiver(new EventReceiver()) {
      _receiver()->moveToThread(QApplication::instance()->thread());
    }

    void execute() { _f(); }

        QObject *receiver() const { return _receiver.get(); }
    };

    class EventReceiver : public QObject
    {
    public:
        bool event(QEvent *event) override {
            auto myEvent = dynamic_cast<SchedulerEvent*>(event);
            if (myEvent) {
                myEvent->execute();
                return true;
            }
            return false;
        }
    };

public:
    template <typename F>
  void operator()(std::chrono::system_clock::time_point /*when*/, F&& f) {
    auto event = new SchedulerEvent(std::forward<F>(f));
        QApplication::postEvent(event->receiver(), event);
    }
};

```
Future interface
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

    template <typename F>
    auto recover(F&& f) const&; // -> future<result_of_t<F(T)>>

    template <typename S, typename F>
    auto recover(S&& s, F&& f) const&; // -> future<result_of_t<F(T)>>

    template <typename F>
    auto recover(F&& f) &&; // -> future<result_of_t<F(T)>>

    template <typename S, typename F>
    auto recover(S&& s, F&& f) &&; // -> future<result_of_t<F(T)>>

    void cancel();

    auto get_try() const&; // -> T == void ? bool : optional<T>
    auto get_try() &&; // -> T == void ? bool : optional<T>
};

template <typename S, typename F, typename... Ts>
auto when_all(S, F, future<Ts>... args); // -> future<result_of_t<F(Ts...)>>

template <typename S, typename F, typename T, typename... Ts>
auto when_any(S, F, future<T>, future<Ts>... args); // -> future<result_of_t<F(T,size_t)>>


template <typename S, // models task scheduler
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures of the same type
auto when_all(S schedule, F f, const std::pair<I, I>& range); // -> future<result_of_t<F(const std::vector<typename std::iterator_traits<I>::value_type::result_type>)>>

template <typename S, // models task scheduler
          typename F, // models functional object
          typename I> // models ForwardIterator that reference to a range of futures of the same type
auto when_any(S schedule, F f, const std::pair<I, I>& range); // -> future<result_of_t<F(typename std::iterator_traits<I>::value_type::result_type, size_t)>>


template <typename Sig, typename S, typename F>
auto package(S s, F f); // -> pair<packaged_task<Sig>, future<result_of_t<Sig>>>;

template <typename S, typename F, typename ...Args>
auto async(S s, F&& f, Args&&... args) -> future<std::result_of_t<F (Args...)>>
```

