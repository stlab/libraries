/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CHANNEL_HPP
#define STLAB_CHANNEL_HPP

#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <memory>
#include <tuple>
#include <utility>

#include <stlab/future.hpp>
#include <stlab/tuple_algorithm.hpp>


#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

template <typename> class sender;
template <typename> class receiver;

/**************************************************************************************************/
/*
 * close on a process is called when a process is in an await state to signal that no more data is coming. 
 * In response to a close, a process can switch to a yield state to yield values, otherwise it is destructed.
 * await_try is await if a value is available, otherwise yield (allowing for an interruptible task).
 */

enum class process_state {
    await,
    yield,
};

using process_state_scheduled = std::pair<process_state, std::chrono::system_clock::time_point>;

constexpr process_state_scheduled await_forever {
    process_state::await,
    std::chrono::system_clock::time_point::max()
};

constexpr process_state_scheduled yield_immediate {
    process_state::yield,
    std::chrono::system_clock::time_point::min()
};

/**************************************************************************************************/

struct buffer_size
{
    const size_t _value;
    explicit buffer_size(size_t v) : _value(v) {}
};

/**************************************************************************************************/

template <typename I, // I models ForwardIterator
          typename N, // N models PositiveInteger
          typename F> // F models UnaryFunction
I for_each_n(I p, N n, F f) {
    for (N i = 0; i != n; ++i, ++p) {
        f(*p);
    }
    return p;
}

struct identity {
    template <typename T>
    T operator()(T x) { return x; }
};

/**************************************************************************************************/

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> { using type = R; };

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

template <class T1, class ...T>
struct first_t
{
    using type = T1;
};
    
/**************************************************************************************************/

template <typename> struct argument_of;
template <typename R, typename Arg>
struct argument_of<R(Arg)> { using type = Arg; };

template <typename T>
using argument_of_t = typename argument_of<T>::type;

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

class avoid_ { };

template <typename T>
using avoid = std::conditional_t<std::is_same<void, T>::value, avoid_, T>;

/**************************************************************************************************/

template <typename F, typename... Args>
auto avoid_invoke(F&& f, Args&&... args)
        -> std::enable_if_t<!std::is_same<void,
            decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::value,
            decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <typename F, typename... Args>
auto avoid_invoke(F&& f, Args&&... args)
        -> std::enable_if_t<std::is_same<void,
            decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::value,
            avoid_>
{
    std::forward<F>(f)(std::forward<Args>(args)...);
    return avoid_();
}

/**************************************************************************************************/

template <typename T>
using receiver_type = typename std::remove_reference_t<T>::result_type;

/**************************************************************************************************/

// REVISIT (sparent) : I have a make_weak_ptr() someplace already. Should be in memory.hpp

template <typename T>
auto make_weak_ptr(const std::shared_ptr<T>& x) { return std::weak_ptr<T>(x); }

/**************************************************************************************************/

template <typename T>
struct shared_process_receiver {
    virtual ~shared_process_receiver() = default;

    virtual void map(sender<T>) = 0;
    virtual void clear_to_send() = 0;
    virtual void add_receiver() = 0;
    virtual void remove_receiver() = 0;
    virtual timed_schedule_t scheduler() const = 0;
    virtual void set_buffer_size(size_t) = 0;
    virtual size_t buffer_size() const = 0;
};

/**************************************************************************************************/

template <typename T>
struct shared_process_sender {
    virtual ~shared_process_sender() = default;

    virtual void send(avoid<T> x) = 0;
    virtual void add_sender() = 0;
    virtual void remove_sender() = 0;
};

/**************************************************************************************************/

// the following is based on the C++ standard proposal N4502

#if __GNUC__ < 5 && ! defined __clang__
// http://stackoverflow.com/a/28967049/1353549
template <typename...>
struct voider
{
    using type = void;
};
template <typename...Ts>
using void_t = typename voider<Ts...>::type;
#else
template <typename...>
using void_t = void;
#endif

// Primary template handles all types not supporting the operation.
template <typename, template <typename> class, typename = void_t<>>
struct detect : std::false_type {};

// Specialization recognizes/validates only types supporting the archetype.
template <typename T, template <typename> class Op>
struct detect<T, Op, void_t<Op<T>>> : std::true_type {};

/**************************************************************************************************/

template <typename T>
using process_close_t = decltype(std::declval<T&>().close());

template <typename T>
constexpr bool has_process_close_v = detect<T, process_close_t>::value;

template <typename T>
auto process_close(T& x) -> std::enable_if_t<has_process_close_v<T>> {
    x.close();
}

template <typename T>
auto process_close(T& x) -> std::enable_if_t<!has_process_close_v<T>> { }

/**************************************************************************************************/

template <typename T>
using process_state_t = decltype(std::declval<const T&>().state());

template <typename T>
constexpr bool has_process_state_v = detect<T, process_state_t>::value;

template <typename T>
auto get_process_state(const T& x) -> std::enable_if_t<has_process_state_v<T>, process_state_scheduled> {
    return x.state();
}

template <typename T>
auto get_process_state(const T& x) -> std::enable_if_t<!has_process_state_v<T>, process_state_scheduled> {
    return await_forever;
}

/**************************************************************************************************/

template <typename T>
using process_yield_t = decltype(std::declval<T&>().yield());

template <typename T>
constexpr bool has_process_yield_v = detect<T, process_yield_t>::value;

/**************************************************************************************************/

template <typename T, typename...>
auto yield_type_(decltype(&T::yield)) -> decltype(std::declval<T>().yield());

template <typename T, typename... Arg>
auto yield_type_(...) -> decltype(std::declval<T>()(std::declval<Arg>()...));

template <typename T, typename... Arg>
using yield_type = decltype(yield_type_<T, Arg...>(0));

/**************************************************************************************************/


template <typename P, typename...T, std::size_t... I>
void await_args_(P& p, std::tuple<T...>& args, std::index_sequence<I...>) {
    p.await(std::move(std::get<I>(args))...);
}

template <typename P, typename... T>
void await_args(P& p, std::tuple<T...>& args) {
    await_args_(p, args, std::make_index_sequence<sizeof...(T)>());
}

template <typename T>
struct default_queue_strategy
{
    using value_type = std::tuple<T>;

    std::deque<T> _queue;

    bool empty() const {
        return _queue.empty();
    }

    auto front() {
        return std::make_tuple(std::move(_queue.front()));
    }

    void pop_front() {
        _queue.pop_front();
    }

    auto size() const {
        return std::array<std::size_t, 1>{_queue.size()};
    }

    void append(std::size_t, T t) {
        _queue.emplace_back(std::move(t));
    }
};


template <typename... T>
struct join_queue_strategy
{
    static const std::size_t Size = sizeof...(T);
    using value_type = std::tuple<T...>;
    using queue_size_t = std::array<std::size_t, Size>;


    std::tuple<std::deque<T>...> _queue;

    bool empty() const { return tuple_find(_queue, [](const auto& c) { return c.empty(); }) != Size; }

    template <std::size_t... I, typename U>
    auto front_impl(U& u, std::index_sequence<I...>) {
        return std::make_tuple(std::move(std::get<I>(u)...));
    }

    auto front() {
        assert(!empty && "front on an empty container is a very bad idea!");
        return front_impl(_queue, std::make_index_sequence<Size>());
    }

    void pop_front() {
        tuple_for_each(_queue, [](auto& c) {c.pop_front(); });
    }

    auto size() const {
        queue_size_t result;
        std::size_t i = 0;
        tuple_for_each(_queue, [&i, &result](const auto& c) { result[i++] = c.size(); });
        return result;
    }

    template <std::size_t I, typename U>
    struct appender
    {
        static void append(std::size_t index, U&& u) {
            if (index == I) std::get<I>(_queue).emplace_back(std::forward<U>(u));
            else appender<I + 1, U>::append(index, std::forward<U>(u));
        }
    };

    template <typename U>
    void append(std::size_t index, U&& u) {
        assert(index < Size &&  "Index out of bounds");
        appender<0, U>::append(index, std::forward<U>(u));
    }

};

/**************************************************************************************************/

template <typename Q, typename SP, typename Arg>
struct shared_process_sender_ : public shared_process_sender<Arg>
{
    Q&                  _queue;
    SP&                 _shared_process;
    const std::size_t   _index;

    template <std::size_t... I>
    shared_process_sender_(Q& q, SP& sp, std::index_sequence<I...>) 
        : _queue(q)
        , _shared_process(sp)
        , _index(I...)
    {}

    void add_sender() override {
        ++_shared_process._sender_count;
    }

    void remove_sender() override {
        assert(_shared_process._sender_count > 0);
        if (--_shared_process._sender_count == 0) {
            bool do_run;
            {
                std::unique_lock<std::mutex> lock(_shared_process._process_mutex);
                _shared_process._process_close_queue = true;
                do_run = !_shared_process._receiver_count && !_shared_process._process_running;
                _shared_process._process_running = _shared_process._process_running || do_run;
            }
            if (do_run) _shared_process.run();
        }
    }

    void send(Arg arg) override {
        bool do_run;
        {
            std::unique_lock<std::mutex> lock(_shared_process._process_mutex);
            _queue.append(_index,std::move(arg)); // TODO (sparent) : overwrite here.

            do_run = !_shared_process._receiver_count && !_shared_process._process_running;
            _shared_process._process_running = _shared_process._process_running || do_run;
        }
        if (do_run) _shared_process.run();
    }
};


template<typename Q, typename T, typename... Args>
struct shared_process : shared_process_receiver<yield_type<T, Args...>>,
                        std::enable_shared_from_this<shared_process<Q, T, Args...>>,
                        shared_process_sender_<Q, shared_process<Q, T, Args...>,Args>... {

    /*
        the downstream continuations are stored in a deque so we don't get reallocations
        on push back - this allows us to make calls while additional inserts happen.
    */

    using result            = yield_type<T, Args...>;
    using queue_strategy    = Q;

    std::mutex                   _downstream_mutex;
    std::deque<sender<result>>   _downstream;
    queue_strategy           _queue;

    timed_schedule_t         _scheduler;
    T                        _process;

    std::mutex               _process_mutex;

    bool                     _process_running = false;
    std::atomic_size_t       _process_suspend_count{ 0 };
    bool                     _process_close_queue = false;
    // REVISIT (sparent) : I'm not certain final needs to be under the mutex
    bool                     _process_final = false;

    std::atomic_size_t       _sender_count{0};
    std::atomic_size_t       _receiver_count;

    std::atomic_size_t       _process_buffer_size{ 1 };

    const std::tuple<std::shared_ptr<shared_process_receiver<Args>>...> _upstream;


    template <typename S, typename F>
    shared_process(S&& s, F&& f) : 
        shared_process_sender_(_queue, *this, std::make_index_sequence<sizeof...(Args)>()),
        _scheduler(std::forward<S>(s)),
        _process(std::forward<F>(f))
    {
        _sender_count = 1;
        _receiver_count = !std::is_same<result, void>::value;
    }

    template <typename S, typename F, typename U>
    shared_process(S&& s, F&& f, U&& u) :
        shared_process_sender_(_queue, *this, std::make_index_sequence<sizeof...(Args)>()),
        _scheduler(std::forward<S>(s)),
        _process(std::forward<F>(f)), 
        _upstream(std::forward<U>(u))
    {
        _sender_count = sizeof...(Args);
        _receiver_count = !std::is_same<result, void>::value;
    }

    void add_receiver() override {
        if (std::is_same<result, void>::value) return;
        ++_receiver_count;
    }

    void remove_receiver() override {
        if (std::is_same<result, void>::value) return;
        /*
            NOTE (sparent) : Decrementing the receiver count can allow this to start running on a
            send before we can get to the check - so we need to see if we are already running
            before starting again.
        */
        assert(_receiver_count > 0);
        if (--_receiver_count == 0) {
            bool do_run;
            {
                std::unique_lock<std::mutex> lock(_process_mutex);
                do_run = (!_queue.empty() || _process_close_queue) && !_process_running;
                _process_running = _process_running || do_run;
            }
            if (do_run) run();
        }
    }

    timed_schedule_t scheduler() const override {
        return _scheduler;
    }

    void task_done() {
        bool do_run;
        bool do_final;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            do_run = !_queue.empty() || _process_close_queue;
            _process_running = do_run;
            do_final = _process_final;
        }
        // The mutual exclusiveness of this assert implies too many variables. Should have a single
        // "get state" call.
        assert(!(do_run && do_final) && "ERROR (sparent) : can't run and close at the same time.");
        // I met him on a Monday and my heart stood still
        if (do_run) run();
        if (do_final) {
            std::unique_lock<std::mutex> lock(_downstream_mutex);
            _downstream.clear(); // This will propogate the close to anything downstream
        }
        // REVISIT (sparent) : process needs to be optional so it can be destructed here
        // _process~();
    }

    void clear_to_send() override {
        bool do_run = false;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            --_process_suspend_count; // could be atomic?
            assert(_process_running && "ERROR (sparent) : clear_to_send but not running!");
            if (!_process_suspend_count) {
                // FIXME (sparent): This is calling the process state ender the lock.
                if (get_process_state(_process).first == process_state::yield || !_queue.empty()
                        || _process_close_queue) {
                    do_run = true;
                } else {
                    _process_running = false;
                    do_run = false;
                }
            }
        }
        // Somebody told me that his name was Bill
        if (do_run) run();
    }

    auto pop_from_queue() {
        boost::optional<typename Q::value_type> message; // TODO : make functional
        std::array<bool, sizeof...(Args)> do_cts = {false};
        bool do_close = false;

        std::unique_lock<std::mutex> lock(_process_mutex);
        if (_queue.empty()) {
            std::swap(do_close, _process_close_queue);
            _process_final = do_close; // unravel after any yield
        }
        else {
            message = std::move(_queue.front());
            _queue.pop_front();
            auto queue_size = _queue.size();
            std::size_t i{ 0 };
            std::for_each(queue_size.begin(), queue_size.end(),
                [&do_cts, &i,this](const auto& size) {
                do_cts[i] = size <= (_process_buffer_size - 1);
                ++i;
            });
        }
        return std::make_tuple(std::move(message), do_cts, do_close);
    }


    bool dequeue() {
        boost::optional<typename Q::value_type> message; // TODO : make functional
        std::array<bool, sizeof...(Args)> do_cts;
        bool do_close = false;

        std::tie(message, do_cts, do_close) = pop_from_queue();

        std::size_t i = 0;
        tuple_for_each(_upstream, [do_cts,&i](const auto& u) {
            if (do_cts[i] && u) u->clear_to_send();
            ++i;
        });

        if (message) await_args(_process, std::move(message.get()));
        else if (do_close) process_close(_process);
        return bool(message);
    }

    template <typename U>
    auto step() -> std::enable_if_t<has_process_yield_v<U>> {
        /*
            While we are awaiting we will flush the queue. The assumption here is that work
            is done on yield()
        */
        while (get_process_state(_process).first == process_state::await) {
            if (!dequeue()) break;
        }

        auto now = std::chrono::system_clock::now();
        process_state state;
        std::chrono::system_clock::time_point when;
        std::tie(state, when) = get_process_state(_process);

        /*
            Once we hit yield, go ahead and call it. If the yield is delayed then schedule it. This
            process will be considered running until it executes.
        */
        if (state == process_state::yield) {
            if (when <= now) broadcast(_process.yield());
            else _scheduler(when, [_this = this->shared_from_this()] {
                _this->broadcast(_this->_process.yield());
            });
        }

        /*
            We are in an await state and the queue is empty.
            
            If we await forever then task_done() leaving us in an await state.
            else if we await with an expired timeout then go ahead and yield now.
            else schedule a timeout when we will yield if not canceled by intervening await.
        */
        else if (when == std::chrono::system_clock::time_point::max()) {
            task_done();
        } else if (when <= now) {
            broadcast(_process.yield());
        } else {
            /*
                REVISIT (sparent) : The case is not implemented.

                Schedule a timeout. If a new value is received then cancel pending timeout.

                Mechanism for cancelation? Possibly a shared/weak ptr. Checking yield state is
                not sufficient since process might have changed state many times before timeout
                is invoked.
                
                Timeout may occur concurrent with other operation - requires syncronization.
            */
            assert(false && "await with non-max/min timout not yet supported.");


            #if 0 // This logic is flawed
            if (get_process_state(_process).first == process_state::await && 
                get_process_state(_process).second > std::chrono::system_clock::now())
            {
                _scheduler(get_process_state(_process).second, [_this = this->shared_from_this()]{
                    if (get_process_state(_this->_process).first != process_state::yield)
                    {
                        _this->broadcast(_this->_process.yield());
                    }
                });
            }
            else {
                task_done();
            }
            #endif
        }
    }

    /*
        REVISIT (sparent) : See above comments on step() and ensure consistency.
                
        What is this code doing, if we don't have a yield then it also assumes no await?
    */

    template <typename U>
    auto step() -> std::enable_if_t<!has_process_yield_v<U>> {
        boost::optional<typename Q::value_type> message; // TODO : make functional
        std::array<bool, sizeof...(Args)> do_cts;
        bool do_close = false;

        std::tie(message, do_cts, do_close) = pop_from_queue();

        std::size_t i = 0;
        tuple_for_each(_upstream, [do_cts, &i](const auto& u) {
            if (do_cts[i] && u) u->clear_to_send();
            ++i;
        });

        if (message) {
            broadcast(avoid_invoke(_process, std::move(std::get<0>(message.get()))));
        }
        else task_done();
    }

    void run() {
        _scheduler(std::chrono::system_clock::time_point::min(), [_p = make_weak_ptr(this->shared_from_this())]{
            auto p = _p.lock();
            if (p) p->template step<T>();
        });
    }

    template <typename... A>
    void broadcast(const A&... args) {
        /*
            We broadcast the result to any processes currently attached to receiver
        */

        std::size_t n;
        {
            std::unique_lock<std::mutex> lock(_downstream_mutex);
            n = _downstream.size();
        }

        /*
            There is no need for a lock here because the other processes that could change
            _process_suspend_count must all be ready (and not running).
            
            But we'll assert that to be sure!
        */
        // std::unique_lock<std::mutex> lock(_process_mutex);
        assert(_process_suspend_count == 0 && "broadcasting while suspended");

        /*
            Suspend for however many downstream processes we have + 1 for this process - that
            ensures that we are not kicked to run while still broadcasting.
        */

        _process_suspend_count = n + 1;

        /*
            There is no lock on _downstream here. We only ever append to this deque so the first
            n elements are good.
        */

        for_each_n(begin(_downstream), n, [&](const auto& e){
            e(args...);
        });

        clear_to_send(); // unsuspend this process
    }


    void map(sender<result> f) override {
        /*
            REVISIT (sparent) : If we are in a final state then we should destruct the sender
            and not add it here.
        */
        {
            std::unique_lock<std::mutex> lock(_downstream_mutex);
            _downstream.emplace_back(f);
        }
    }

    void set_buffer_size(size_t buffer_size) override {
        _process_buffer_size = buffer_size;
    }

    size_t buffer_size() const override {
        return _process_buffer_size;
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename T, typename S>
auto channel(S s) -> std::pair<sender<T>, receiver<T>> {
    auto p = std::make_shared<detail::shared_process<detail::default_queue_strategy<T>, identity, T>>(std::move(s), identity());
    return std::make_pair(sender<T>(p), receiver<T>(p));
}


/**************************************************************************************************/
#if 0
template <typename S, typename F, typename...R>
auto join(S s, F f, R&&... upstream_receiver){
    auto p = std::make_shared<detail::shared_process<detail::join_queue_strategy<detail::receiver_type<R>...>, 
                                                     F, 
                                                     detail::receiver_type<R>...>>(std::move(s), std::move(F))
    using result_t = detail::yield_type<F, detail::receiver_type<R>...>;
    using context_t = detail::join_context<detail::receiver_type<R>...>;
    using process_t = detail::join_processor<result_t, F, context_t, detail::receiver_type<R>...>;

    auto shared_context = std::make_shared<context_t>();
    auto process = process_t(std::move(f));

    auto receiver_collectors = 
        detail::create_receiver_collectors<detail::join_context_worker_helper>(shared_context,
                                           std::make_index_sequence<sizeof...(R)>(), 
                                           upstream_receiver...);

    auto p = detail::create_process<std::tuple<boost::optional<detail::receiver_type<R>>...>>(
        std::move(s), 
        std::move(process), 
        receiver_collectors,
        std::make_index_sequence<sizeof...(R)>());
    
    detail::map_as_sender<std::tuple<boost::optional<detail::receiver_type<R>>...>>(receiver_collectors, p);

    return receiver<result_t>(std::move(p));
}
#endif

/**************************************************************************************************/

template <typename> class receiver;

template <typename T>
class receiver {
    using ptr_t = std::shared_ptr<detail::shared_process_receiver<T>>;

    ptr_t _p;
    bool _ready = false;

    template <typename U>
    friend class sender;

    template <typename U>
    friend class receiver; // huh?

    template <typename U, typename V>
    friend auto channel(V) -> std::pair<sender<U>, receiver<U>>;
#if 0
    template <typename S, typename F, typename...U>
    friend auto join(S, F, U&&...);

    template <std::size_t I, typename W, typename C, typename R>
    friend auto detail::create_receiver_collector(std::shared_ptr<C>&, R&&);
#endif
    receiver(ptr_t p) : _p(std::move(p)) { }

  public:
    using result_type = T;

    receiver() = default;

    ~receiver() {
        if (!_ready && _p) _p->remove_receiver();
    }

    receiver(const receiver& x) : _p(x._p) {
        if (_p) _p->add_receiver();
    }
    
    receiver(receiver&&) noexcept = default;

    receiver& operator=(const receiver& x) {
        auto tmp = x; *this = std::move(tmp); return *this;
    }
    
    receiver& operator=(receiver&& x) noexcept = default;

    void set_ready() {
        if (!_ready && _p) _p->remove_receiver();
        _ready = true;
    }

    bool ready() const { return _ready; }

    template <typename F>
    auto operator|(F&& f) const {
        // TODO - report error if not constructed or _ready.
        auto p = std::make_shared<detail::shared_process<detail::default_queue_strategy<T>, F, T>>(_p->scheduler(), std::forward<F>(f), _p);
        _p->map(sender<T>(p));
        return receiver<detail::yield_type<F, T>>(std::move(p));
    }

    auto operator|(buffer_size bz) {
        set_buffer_size(bz._value);
        return *this;
    }

    void set_buffer_size(size_t queue_size) {
        _p->set_buffer_size(queue_size);
    }
};

/**************************************************************************************************/

template <typename T>
class sender {
    using ptr_t = std::weak_ptr<detail::shared_process_sender<T>>;
    ptr_t _p;

    template <typename U>
    friend class receiver;

    template <typename U, typename V>
    friend auto channel(V)->std::pair<sender<U>, receiver<U>>;
#if 0
    template <typename C, typename RC, typename P, std::size_t... I>
    friend void detail::map_as_sender_(RC&, P&, std::index_sequence<I...>);
#endif
    sender(ptr_t p) : _p(std::move(p)) {}

  public:
    sender() = default;

    ~sender() {
        auto p = _p.lock();
        if (p) p->remove_sender();
    }

    sender(const sender& x) : _p(x._p) {
        auto p = _p.lock();
        if (p) p->add_sender();
    }

    sender(sender&&) noexcept = default;
    
    sender& operator=(const sender& x) {
        auto tmp = x; *this = std::move(tmp); return *this;
    }

    sender& operator=(sender&&) noexcept = default;
    void close() {
        auto p = _p.lock();
        if (p) p->remove_sender();
        _p.reset();
    }

    template <typename... A>
    void operator()(A&&... args) const {
        auto p = _p.lock();
        if (p) p->send(std::forward<A>(args)...);
    }
};

/**************************************************************************************************/

template <typename F> struct function_process;

template <typename R, typename... Args>
struct function_process<R (Args...)> {
    std::function<R (Args...)> _f;
    std::function<R ()> _bound;
    bool _done = true;

    using signature = R(Args...);

    template <typename F>
    function_process(F&& f) : _f(std::forward<F>(f)) { }

    template <typename... A>
    void await(A&&... args) {
        _bound = std::bind(_f, std::forward<A>(args)...);
        _done = false;
    }

    R yield() { _done = true; return _bound(); }
    process_state state() const { return _done ? process_state::await : process_state::yield; }
};

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
