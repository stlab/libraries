/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CHANNEL_HPP
#define STLAB_CHANNEL_HPP

#include <chrono>
#include <deque>
#include <memory>
#include <tuple>
#include <utility>

#include <stlab/future.hpp>

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
    await_try,
    yield,
};

using process_state_scheduled = std::pair<process_state, std::chrono::system_clock::time_point>;

constexpr process_state_scheduled await_forever {
    process_state::await,
    std::chrono::system_clock::time_point::max()
};

constexpr process_state_scheduled await_try_forever{
    process_state::await_try,
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

template<typename T, typename Arg>
struct shared_process : shared_process_receiver<yield_type<T, Arg>>,
                        shared_process_sender<Arg>,
                        std::enable_shared_from_this<shared_process<T, Arg>> {

    /*
        the downstream continuations are stored in a deque so we don't get reallocations
        on push back - this allows us to make calls while additional inserts happen.
    */

    using argument = Arg;
    using result = yield_type<T, Arg>;

    std::mutex                   _downstream_mutex;
    std::deque<sender<result>>   _downstream;

    timed_schedule_t         _scheduler;
    T                        _process;

    std::mutex               _process_mutex;
    std::deque<argument>     _process_message_queue;
    bool                     _process_running = false;
    std::atomic_size_t       _process_suspend_count{ 0 };
    bool                     _process_close_queue = false;
    // REVISIT (sparent) : I'm not certain final needs to be under the mutex
    bool                     _process_final = false;

    std::atomic_size_t       _sender_count{0};
    std::atomic_size_t       _receiver_count;

    std::atomic_size_t       _process_buffer_size{ 1 };

    const std::deque<std::shared_ptr<shared_process_receiver<argument>>> _upstream;

    template <typename S, typename F>
    shared_process(S&& s, F&& f) : 
        _scheduler(std::forward<S>(s)),
        _process(std::forward<F>(f))
    {
        _sender_count = 1;
        _receiver_count = !std::is_same<result, void>::value;
    }

    template <typename S, typename F, typename... U>
    shared_process(S&& s, F&& f, const U&... p) :
        _scheduler(std::forward<S>(s)),
        _process(std::forward<F>(f)), 
        _upstream(std::initializer_list<std::shared_ptr<shared_process_receiver<argument>>>{p...})
    {
        _sender_count = sizeof...(U);
        _receiver_count = !std::is_same<result, void>::value;
    }

    void add_sender() override {
        ++_sender_count;
    }

    void remove_sender() override {
        assert(_sender_count > 0);
        if (--_sender_count == 0) {
            bool do_run;
            {
                std::unique_lock<std::mutex> lock(_process_mutex);
                _process_close_queue = true;
                do_run = !_receiver_count && !_process_running;
                _process_running = _process_running || do_run;
            }
            if (do_run) run();
        }
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
                do_run = (!_process_message_queue.empty() || _process_close_queue) && !_process_running;
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
            do_run = !_process_message_queue.empty() || _process_close_queue;
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
        // The downstream sender may trigger this because an other parallel upstream
        // process may have just passed down a value
        if (_process_suspend_count == 0)
            return;

        bool do_run = false;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            auto ps = get_process_state(_process);
            --_process_suspend_count; // could be atomic?
            assert(_process_running && "ERROR (sparent) : clear_to_send but not running!");
            if (!_process_suspend_count) {
                // FIXME (sparent): This is calling the process state ender the lock.
                if (ps.first == process_state::yield || !_process_message_queue.empty()
                        || _process_close_queue) {
                    do_run = true;
                } 
                else {
                    _process_running = false;
                    do_run = false;
                }
            }
        }
        // Somebody told me that his name was Bill
        if (do_run) run();
    }

    bool dequeue() {
        boost::optional<argument> message; // TODO : make functional
        bool do_cts = false;
        bool do_close = false;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            if (_process_message_queue.empty()) {
                std::swap(do_close, _process_close_queue);
                _process_final = do_close; // unravel after any yield
            } else {
                message = std::move(_process_message_queue.front());
                _process_message_queue.pop_front();
                do_cts = _process_message_queue.size() <= (_process_buffer_size - 1);
            }
        }

        if (do_cts) {
            for (auto& up : _upstream) { up->clear_to_send(); }
        }

        if (message) _process.await(std::move(message.get()));
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
        
        This code is too similar to dequeue() - refactor.
        
        What is this code doing, if we don't have a yield then it also assumes no await?
        
    */

    template <typename U>
    auto step() -> std::enable_if_t<!has_process_yield_v<U>> {
        boost::optional<argument> message; // TODO : make functional
        bool do_cts = false;
        bool do_close = false;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            if (_process_message_queue.empty()) {
                std::swap(do_close, _process_close_queue);
                _process_final = do_close; // unravel after any yield
            } else {
                message = std::move(_process_message_queue.front());
                _process_message_queue.pop_front();
                do_cts = _process_message_queue.size() <= (_process_buffer_size - 1);
            }
        }

        if (do_cts) {
            for (auto& up : _upstream) { up->clear_to_send(); }
        }
        if (message) {
            broadcast(avoid_invoke(_process, std::move(message.get())));
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

    void send(argument arg) override {
        bool do_run;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            _process_message_queue.emplace_back(std::move(arg)); // TODO (sparent) : overwrite here.
            
            do_run = !_receiver_count && !_process_running;
            _process_running = _process_running || do_run;
        }
        if (do_run) run();
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
    auto p = std::make_shared<detail::shared_process<identity, T>>(std::move(s), identity());
    return std::make_pair(sender<T>(p), receiver<T>(p));
}

/**************************************************************************************************/
namespace detail {


template <std::size_t I, typename W, typename C, typename R>
auto create_receiver_collector(std::shared_ptr<C>& context, R&& upstream_receiver) {
    using context_worker = typename W::template type<I, C>;
    auto receiver_collector = std::forward<R>(upstream_receiver) | context_worker(context);
    return receiver_collector._p;
}

template <typename W, typename C, typename... R, std::size_t... I>
auto create_receiver_collectors(std::shared_ptr<C>& context, std::index_sequence<I...>, R&&... upstream_receiver) {
    return std::make_tuple(create_receiver_collector<I,W>(context, std::forward<R>(upstream_receiver))...);
}

template <typename C, typename S, typename P, typename RC, std::size_t... I>
auto create_process(S&& s, P&& p, RC& receiver_collectors, std::index_sequence<I...>) {
    return std::make_shared<detail::shared_process<P, C>>(
        std::forward<S>(s), std::forward<P>(p), std::get<I>(receiver_collectors)...);
}

template <typename C, typename RC, typename P, std::size_t... I>
void map_as_sender_(RC& receiver_collectors, P& p, std::index_sequence<I...>) {
    (void)std::initializer_list<int>{(std::get<I>(receiver_collectors)->map(sender<C>(p)), 0)...};
}

template <typename C, typename RC, typename P>
void map_as_sender(RC& receiver_collectors, P& p) {
    map_as_sender_<C>(receiver_collectors, p, std::make_index_sequence<std::tuple_size<RC>::value>());
}

template <typename F, typename...T, std::size_t... I>
auto apply_join_args_(const F& f, std::tuple<boost::optional<T>...>& args, std::index_sequence<I...>) {
    return f(std::move(std::get<I>(args)).value()...);
}

template <typename F, typename... T>
auto apply_join_args(const F& f, std::tuple<boost::optional<T>...>& args) {
    return apply_join_args_(f, args, std::make_index_sequence<sizeof...(T)>());
}

template <std::size_t I, typename... T>
void reset_tuple_optional_(std::tuple<boost::optional<T>...>& values) {
    std::get<I>(values) = boost::none;
}

template <typename... T, std::size_t... I>
void reset_tuple_optional(std::tuple<boost::optional<T>...>& values, std::index_sequence<I...>) {
    (void)std::initializer_list<int>{(reset_tuple_optional_<I>(values), 0)...};
}



template <typename... T>
struct join_context
{
    const static std::size_t            _size = sizeof...(T);
    std::tuple<boost::optional<T>...>   _result;
    std::atomic_size_t                  _remaining{ _size };

    template<std::size_t i, typename U>
    void await(U&& u) {
        assert(!std::get<i>(_result).is_initialized());
        std::get<i>(_result) = std::forward<U>(u);
    }

    void reset() {
        reset_tuple_optional(_result, std::make_index_sequence<sizeof...(T)>());
        _remaining = _size;
    }

    std::tuple<boost::optional<T>...> yield() {
        if (--_remaining == 0) {
            auto result = std::move(_result);
            reset();
            return result;
        }
        return _result;
    }

    template <std::size_t I>
    auto state() const {
        if (!std::get<I>(_result).is_initialized()) {
            return await_forever;
        }

        return yield_immediate;
    }
};


template <std::size_t i, typename C>
struct join_context_worker
{
    std::shared_ptr<C> _context;
    mutable bool       _send{ false };

    explicit join_context_worker(const std::shared_ptr<C>& c) : _context(c) {}

    auto state() const {
        auto s = _context->template state<i>();
        if (_send && s.first == process_state::await) {
            _send = false;
        }
        if (_send) return await_try_forever;
        return s;
    }

    template <typename U>
    void await(U&& u) { _context->template await<i, U>(std::forward<U>(u)); }

    auto yield() { _send = true; return _context->yield(); }
};


template <typename R, typename F, typename C, typename...T>
struct join_processor
{
    F                                 _f;
    std::size_t                       _remaining = sizeof...(T);
    std::tuple<boost::optional<T>...> _input;
    boost::optional<R>                _result;

    explicit join_processor(F&& f) : _f(std::move(f)) {}

    template <std::size_t I>
    void store_input(std::tuple<boost::optional<T>...>& input) {
        if (std::get<I>(input).is_initialized() && !std::get<I>(_input).is_initialized()) {
            std::get<I>(_input) = std::move(std::get<I>(input).get());
        }
    }

    template <std::size_t... I>
    void store_inputs(std::tuple<boost::optional<T>...>& input, std::index_sequence<I...>) {
        (void)std::initializer_list<int>{(store_input<I>(input), 0)...};
    }

    void await(std::tuple<boost::optional<T>...> input) {
        store_inputs(input, std::make_index_sequence<sizeof...(T)>());
        if (--_remaining == 0) {
            _result = detail::apply_join_args(_f, _input);
            reset_tuple_optional(_input, std::make_index_sequence<sizeof...(T)>());
        }
    }

    R yield() {
        auto r = std::move(_result.get());
        _result = boost::none;
        _remaining = sizeof...(T);
        return r;
    }

    auto state() const {
        if (_result.is_initialized())
            return yield_immediate;

        return await_forever;
    }
};


struct join_context_worker_helper
{
    template <std::size_t I, typename C>
    using type = join_context_worker<I, C>;
};

} // namespace detail

/**************************************************************************************************/

template <typename S, typename F, typename...R>
auto join(S s, F f, R&&... upstream_receiver){
    
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

    template <typename S, typename F, typename...U>
    friend auto join(S, F, U&&...);

    template <std::size_t I, typename W, typename C, typename R>
    friend auto detail::create_receiver_collector(std::shared_ptr<C>&, R&&);

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
        auto p = std::make_shared<detail::shared_process<F, T>>(_p->scheduler(), std::forward<F>(f), _p);
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

    template <typename C, typename RC, typename P, std::size_t... I>
    friend void detail::map_as_sender_(RC&, P&, std::index_sequence<I...>);

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
