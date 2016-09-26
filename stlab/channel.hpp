/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CHANNEL_HPP
#define STLAB_CHANNEL_HPP

#include <deque>
#include <memory>
#include <tuple>
#include <utility>

#include <boost/variant.hpp>

#include <stlab/future.hpp>
#include <stlab/scopeguard.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

template <typename> class sender;
template <typename> class receiver;

/**************************************************************************************************/

enum class process_state {
    await,
    hold,
    yield,
    closed
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
    virtual void clear_hold() = 0;
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
    virtual void send(std::exception_ptr) = 0;
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
auto get_process_state(const T& x) -> std::enable_if_t<has_process_state_v<T>, std::pair<process_state, std::chrono::system_clock::time_point>> {
    return x.state();
}

template <typename T>
auto get_process_state(const T&) -> std::enable_if_t<!has_process_state_v<T>, std::pair<process_state, std::chrono::system_clock::time_point>> {
    return std::make_pair(process_state::await, std::chrono::system_clock::time_point());
}

/**************************************************************************************************/

template <typename T>
using process_set_error_t = decltype(std::declval<T&>().set_error(std::exception_ptr()));

template <typename T>
constexpr bool has_set_process_error_v = detect<T, process_set_error_t>::value;

template <typename T>
auto set_process_error(T& x, std::exception_ptr error) -> std::enable_if_t<has_set_process_error_v<T>, void> {
    x.set_error(std::move(error));
}

template <typename T>
auto set_process_error(T&, std::exception_ptr) -> std::enable_if_t<!has_set_process_error_v<T>, void> {
}

/**************************************************************************************************/

template <typename T>
using process_yield_t = decltype(std::declval<T&>().yield());

template <typename T>
constexpr bool has_process_yield_v = detect<T, process_yield_t>::value;

/**************************************************************************************************/

template <typename T, typename>
auto yield_type_(decltype(&T::yield)) -> decltype(std::declval<T>().yield());

template <typename T, typename Arg>
auto yield_type_(...) -> decltype(std::declval<T>()(std::declval<Arg>()));

template <typename T, typename Arg>
using yield_type = decltype(yield_type_<T, Arg>(0));

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

    enum class message_t { argument, error};
    std::deque<boost::variant<argument, std::exception_ptr>>  _process_message_queue;
    bool                     _process_running = false;
    std::size_t              _process_suspend_count = 0;
    bool                     _process_close_queue = false;
    std::atomic_bool         _process_hold{false};
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

    void clear_hold() override {
        if (_process_hold && get_process_state(_process).first == process_state::await) {
            _process_hold = false;
            task_done();
        }
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
        bool do_run = false;
        {
            auto ps = get_process_state(_process);
            std::unique_lock<std::mutex> lock(_process_mutex);
            assert(_process_suspend_count > 0);
            --_process_suspend_count; // could be atomic?
            assert(_process_running && "ERROR (sparent) : clear_to_send but not running!");
            if (!_process_suspend_count) {
                if (ps.first == process_state::yield || !_process_message_queue.empty()
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
        boost::optional<boost::variant<argument, std::exception_ptr>> message; // TODO : make functional
        bool do_cts = false;
        bool do_close = false;

        std::unique_lock<std::mutex> lock(_process_mutex);
        if (_process_message_queue.empty()) {
            std::swap(do_close, _process_close_queue);
            _process_final = do_close; // unravel after any yield
        }
        else {
            message = std::move(_process_message_queue.front());
            _process_message_queue.pop_front();
            do_cts = _process_message_queue.size() <= (_process_buffer_size - 1);
        }

        return std::make_tuple(std::move(message), do_cts, do_close);
    }
    

    bool dequeue() {
        boost::optional<boost::variant<argument,std::exception_ptr>> message; // TODO : make functional
        bool do_cts = false;
        bool do_close = false;

        std::tie(message, do_cts, do_close) = pop_from_queue();

        if (do_cts) {
            for (auto& up : _upstream) { up->clear_to_send(); }
        }

        if (message) {
            if (message_t::argument == static_cast<message_t>(message.get().which()))
            {
                try {
                    _process.await(std::move(boost::get<argument>(message.get())));
                }
                catch (...) {
                    broadcast(std::move(std::current_exception()));
                }
            }
            else {
                if (has_set_process_error_v<T>)
                    set_process_error(_process, std::move(boost::get<std::exception_ptr>(message.get())));
                else
                    do_close = true;
            }
        }
        else if (do_close) process_close(_process);
        return bool(message);
    }

    template <typename U>
    auto step() -> std::enable_if_t<has_process_yield_v<U>> {
        while (get_process_state(_process).first == process_state::await) {
            if (!dequeue()) break;
        }
        if (get_process_state(_process).first == process_state::yield) {
            broadcast(_process.yield());
            for (auto& u : _upstream) {
                u->clear_hold();
            }
            clear_to_send();
        }
        else {
            if (get_process_state(_process).first == process_state::await && 
                get_process_state(_process).second > std::chrono::system_clock::time_point())
            {
                _scheduler(get_process_state(_process).second, [_this = this->shared_from_this()]{
                    if (get_process_state(_this->_process).first != process_state::yield &&
                        get_process_state(_this->_process).first != process_state::closed) {
                        _this->broadcast(_this->_process.yield());
                        for (auto& u : _this->_upstream) {
                            u->clear_hold();
                        }
                        _this->clear_to_send();
                        //TODO move all to own method
                    }
                });
            }
            else if (get_process_state(_process).first == process_state::hold) {
                _process_hold = true;
                _process_suspend_count = _downstream.size();
            }
            else {
                task_done();
            }
        }
    }

    template <typename U>
    auto step() -> std::enable_if_t<!has_process_yield_v<U>> {
        boost::optional<boost::variant<argument, std::exception_ptr>> message; // TODO : make functional
        bool do_cts = false;
        bool do_close = false;

        std::tie(message, do_cts, do_close) = pop_from_queue();

        if (do_cts) {
            for (auto& up : _upstream) { up->clear_to_send(); }
        }
        if (message) {
            if (message_t::argument == static_cast<message_t>(message.get().which())) {
                try {
                    broadcast(avoid_invoke(_process, std::move(boost::get<argument>(message.get()))));
                }
                catch (...) {
                    broadcast(std::move(std::current_exception()));
                }
                for (auto& u : _upstream) {
                    u->clear_hold();
                }
                clear_to_send();
            } else {
                if (has_set_process_error_v<T>)
                    set_process_error(_process, std::move(boost::get<std::exception_ptr>(message.get())));
                else
                    do_close = true;
            }
        }
        else if (get_process_state(_process).first == process_state::hold) {
            _process_hold = true;
            _process_suspend_count = _downstream.size();
        }
        else {
            task_done();
        }
    }

    void run() {
        _scheduler(std::chrono::system_clock::time_point(), 
            [_p = make_weak_ptr(this->shared_from_this())]{
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

        {
            /*
                There is no need for a lock here because the other processes that could change
                _process_suspend_count must all be ready (and not running).
                
                But we'll assert that to be sure!
            */
            // std::unique_lock<std::mutex> lock(_process_mutex);
            assert(_process_suspend_count == 0 && "broadcasting while suspended");
            _process_suspend_count = n + 1;
        }
        
        // FP: Why use a lock to set n above, but not here?
        for_each_n(begin(_downstream), n, [&](const auto& e){
            e(args...);
        });
    }
    
    void send(std::exception_ptr error) override {
        enqueue(std::move(error));
    }

    void send(argument arg) override {
        enqueue(std::move(arg));
    }

    template <typename U>
    void enqueue(U&& arg)
    {
        bool do_run;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            _process_message_queue.emplace_back(std::forward<U>(arg)); // TODO (sparent) : overwrite here.

            do_run = !_receiver_count && !_process_running;
            _process_running = _process_running || do_run;

#if 0
            running = running || _process_suspend_count;
#endif
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


template <typename... T>
struct join_context : std::enable_shared_from_this<join_context<T...>>
{
    std::tuple<boost::optional<T>...>    _result;
    std::atomic_size_t                   _remaining{ sizeof...(T) + 1};
    std::atomic_size_t                   _sender_index{ std::numeric_limits<size_t>::max() };
    bool                                 _closed = false;
    std::atomic_size_t                   _close_counter{ sizeof...(T) };

    template <std::size_t I>
    void reset_context_result() {
        std::get<I>(_result) = boost::none;
    }

    template <std::size_t... I>
    void reset_context_results(std::index_sequence<I...>) {
        (void)std::initializer_list<int>{(reset_context_result<I>(), 0)...};
    }

    template<std::size_t i, typename U>
    void await(U&& u) {
        assert(!std::get<i>(_result).is_initialized());
        std::get<i>(_result) = std::forward<U>(u);
        if (--_remaining == 1) { _sender_index = i; }
    }

    void reset() {
        reset_context_results(std::make_index_sequence<sizeof...(T)>());
        _sender_index = std::numeric_limits<size_t>::max();
        _close_counter = sizeof...(T);
        _remaining = sizeof...(T) + 1;
    }

    std::shared_ptr<join_context<T...>> yield() {
        assert(_remaining == 1);
        --_remaining;
        return this->shared_from_this();
    }

    void close() { 
        // FP Not sure, if _close_counter is still needed
        if (--_close_counter == 0) _closed = true;
    }

    template <std::size_t I>
    auto state() const {
        // only for that processor the state is yield, that set the last value into _result
        if (_closed)
            return std::make_pair(process_state::closed, std::chrono::system_clock::time_point());

        if (_remaining.load() == 1 && I == _sender_index.load()) {
            return std::make_pair(process_state::yield, std::chrono::system_clock::time_point());
        }

        if (_remaining.load() == 0 || (_remaining.load() > 0 && _sender_index.load() != I && std::get<I>(_result).is_initialized()) ) {
            return std::make_pair(process_state::hold, std::chrono::system_clock::time_point());
        }

        if (_remaining.load() > 1 && !std::get<I>(_result).is_initialized()) {
            return std::make_pair(process_state::await, std::chrono::system_clock::time_point());
        }
        // TODO FP all conditions covered?
        assert(0);
    }
};

template <std::size_t i, typename C>
struct context_worker
{
    std::shared_ptr<C> _context;

    explicit context_worker(const std::shared_ptr<C>& c) : _context(c) {}

    // auto as return type does not work here with VS 2015 Update 3. 
    // The trait inside get_process_state does not see it, grmpf
    std::pair<process_state, std::chrono::system_clock::time_point> state() const { 
        auto s = _context->template state<i>(); 
        return s;
    }
    
    template <typename U>
    void await(U&& u) { _context->template await<i,U>(std::forward<U>(u)); }

    auto yield() { return _context->yield(); }

    void close() { _context->close(); }
};


template <std::size_t I, typename C, typename R>
auto create_receiver_collector(std::shared_ptr<C>& context, R&& upstream_receiver) {
    auto receiver_collector = std::forward<R>(upstream_receiver) | buffer_size(1) | context_worker<I, C>(context);
    return receiver_collector._p;
}

template <typename C, typename... R, std::size_t... I>
auto create_receiver_collectors(std::shared_ptr<C>& context, std::index_sequence<I...>, R&&... upstream_receiver) {
    return std::make_tuple(create_receiver_collector<I>(context, std::forward<R>(upstream_receiver))...);
}

template <typename C, typename S, typename P, typename RC, std::size_t... I>
auto create_joined_process(S&& s, P&& p, RC& receiver_collectors, std::index_sequence<I...>) {
    return std::make_shared<detail::shared_process<P, std::shared_ptr<C>>>(
        std::forward<S>(s), std::forward<P>(p), std::get<I>(receiver_collectors)...);
}

template <typename C, typename RC, typename P, std::size_t... I>
void map_as_sender_(RC& receiver_collectors, P& p, std::index_sequence<I...>) {
    (void)std::initializer_list<int>{(std::get<I>(receiver_collectors)->map(sender<std::shared_ptr<C>>(p)), 0)...};
}

template <typename C, typename RC, typename P>
void map_as_sender(RC& receiver_collectors, P& p) {
    map_as_sender_<C>(receiver_collectors, p, std::make_index_sequence<std::tuple_size<RC>::value>());
}

template <typename F, typename C, std::size_t... I>
auto apply_join_args_(const F& f, std::shared_ptr<C>& context, std::index_sequence<I...>) {
    return f(std::move(std::get<I>(context->_result)).value()...);
}

template <typename F, typename C>
auto apply_join_args(const F& f, std::shared_ptr<C>& context) {
    return apply_join_args_(f, context, std::make_index_sequence<std::tuple_size<decltype(context->_result)>::value>());
}


template <typename R, typename F, typename C>
struct join_processor
{
    F                   _f;
    std::shared_ptr<C>  _context;
    bool                _closed = false;
    explicit join_processor(F&& f) : _f(std::move(f)) {}

    void await(const std::shared_ptr<C>& context) {
        // FP check here the ref counts, to handle a closed upstream ?
        _context = context;
    }

    R yield() {
        auto wc = make_weak_ptr(_context);
        auto c = wc.lock();
        SCOPE_EXIT{ if (c) c->reset(); };
        _context.reset();
        
        return detail::apply_join_args(_f, c);;
    }

    void close() {
        _closed = true;
        _context.reset();
    }

    auto state() const {
        if (_closed)
            return std::make_pair(process_state::closed, std::chrono::system_clock::time_point());

        return std::make_pair((_context? process_state::yield: process_state::await), std::chrono::system_clock::time_point());
    }
};


} // namespace detail

template <typename S, typename F, typename...R>
auto join(S s, F f, R&&... upstream_receiver){
    using result_t = typename std::result_of<F(detail::receiver_type<R>...)>::type;
    using context_t = detail::join_context<detail::receiver_type<R>...>;
    using process_t = detail::join_processor<result_t, F, context_t>;

    auto shared_context = std::make_shared<context_t>();
    auto process = process_t(std::move(f));

    auto receiver_collectors = 
        detail::create_receiver_collectors(shared_context, 
                                           std::make_index_sequence<sizeof...(R)>(), 
                                           upstream_receiver...);

    auto p = detail::create_joined_process<context_t>(
        std::move(s), 
        std::move(process), 
        receiver_collectors,
        std::make_index_sequence<sizeof...(R)>());
    
    detail::map_as_sender<context_t>(receiver_collectors, p);

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

    template <std::size_t I, typename C, typename R>
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

    void operator()(std::exception_ptr error) {
        auto p = _p.lock();
        if (p) p->send(std::move(error));
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
