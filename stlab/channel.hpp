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
#include <thread>

#include <stlab/future.hpp>

/**************************************************************************************************/

namespace stlab {

template <typename I, // I models ForwardIterator
          typename N, // N models PositiveInteger
          typename F> // F models UnaryFunction
I for_each_n(I p, N n, F f) {
    for (N i = 0; i != n; ++i, ++p) {
        f(*p);
    }
    return p;
}

template <typename> class channel;

enum class process_state {
    await,
    await_try,
    yield,
};

/**************************************************************************************************/

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> { using type = R; };

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

/**************************************************************************************************/

template <typename T>
using signature_t = typename T::signature;

/**************************************************************************************************/

template <typename> struct argument_of;
template <typename R, typename Arg>
struct argument_of<R(Arg)> { using type = Arg; };

template <typename T>
using argument_of_t = typename argument_of<T>::type;

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <typename> struct shared_process;

/**************************************************************************************************/

// REVISIT (sparent) : I have a make_weak_ptr() someplace already. Should be in memory.hpp

template <typename T>
auto make_weak_ptr(const std::shared_ptr<T>& x) { return std::weak_ptr<T>(x); }

/**************************************************************************************************/

template <typename T>
struct shared_process_receiver {
    virtual ~shared_process_receiver() { }

    virtual void map(channel<T>) = 0;
    virtual void cts() = 0;
};

/**************************************************************************************************/

template <typename T>
struct shared_process_channel {
    virtual ~shared_process_channel() { }

    virtual void send(T x) = 0;
    virtual void add_channel() = 0;
    virtual void remove_channel() = 0;
};


/**************************************************************************************************/

template<typename T>
struct shared_process : shared_process_receiver<result_of_t_<signature_t<T>>>,
                        shared_process_channel<argument_of_t<signature_t<T>>>,
                        std::enable_shared_from_this<shared_process<T>> {

    /*
        the downstream continuations are stored in a deque so we don't get reallocations
        on push back - this allows us to make calls while additional inserts happen.
    */

    using argument = argument_of_t<signature_t<T>>;
    using result = result_of_t_<signature_t<T>>;

    std::mutex                    _downstream_mutex;
    std::deque<channel<result>>   _downstream;

    T                        _process;

    std::mutex               _process_mutex;
    std::deque<argument>     _process_message_queue;
    bool                     _process_running = false;
    std::size_t              _process_suspend_count = 0;
    bool                     _process_close_queue = false;
    // REVISIT (sparent) : I'm not certain final needs to be under the mutex
    bool                     _process_final = false;

    std::atomic_size_t       _channel_count;

    /*
        Join is not yet implemented so single upstream for now. We don't just use
        a receiver for upstream because we need to be be able to kick the task
        chaining.
    */
    const std::shared_ptr<shared_process_receiver<argument>> _upstream;

    template <typename F>
    shared_process(F&& f) :
        _process(std::forward<F>(f))
    { _channel_count = 1; }

    template <typename F>
    shared_process(F&& f, std::shared_ptr<shared_process_receiver<argument>> p) :
        _process(std::forward<F>(f)),
        _upstream(std::move(p))
    { _channel_count = 1; }

    void add_channel() override {
        ++_channel_count;
    }

    void remove_channel() override {
        if (--_channel_count == 0) {
            bool running = true;
            {
                std::unique_lock<std::mutex> lock(_process_mutex);
                _process_close_queue = true;
                std::swap(running, _process_running);
                running = running || _process_suspend_count;
            }
            if (!running) run();
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

    void cts() override {
        bool do_run = false;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            --_process_suspend_count; // could be atomic?
            assert(_process_running && "ERROR (sparent) : cts but not running!");
            if (!_process_suspend_count) {
                if (_process.state() == process_state::yield || !_process_message_queue.empty()
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

    bool dequeue() {
        boost::optional<argument> message; // TODO : make functional
        bool cts = false;
        bool do_close = false;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            if (_process_message_queue.empty()) {
                std::swap(do_close, _process_close_queue);
                _process_final = do_close; // unravel after any yield
            } else {
                message = std::move(_process_message_queue.front());
                _process_message_queue.pop_front();
                cts = _process_message_queue.size() == 0; // TODO (sparent) : queue size - 1.
            }
        }
        if (cts && _upstream) _upstream->cts();
        if (message) _process.await(std::move(message.get()));
        else if (do_close) _process.close();
        return bool(message);
    }

    void step() {
        /*
            REVISIT (sparent) : Put a timer on this loop to limit it?
        */
        while (_process.state() != process_state::yield) {
            if (!dequeue()) break;
        }
        if (_process.state() == process_state::await) {
            task_done();
        } else {
            broadcast(_process.yield()); // after this point must call cts()
            cts();
        }
    }

    void run() {
        default_scheduler()([_p = make_weak_ptr(this->shared_from_this())]{
            auto p = _p.lock();
            if (p) p->step();
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

        for_each_n(begin(_downstream), n, [&](const auto& e){
            e(args...);
        });
    }

    void send(argument arg) override {
        bool running = true;
        {
            std::unique_lock<std::mutex> lock(_process_mutex);
            _process_message_queue.emplace_back(std::move(arg)); // TODO (sparent) : overwrite here.
            std::swap(running, _process_running);
            running = running || _process_suspend_count;
        }
        if (!running) run();
    }

    void map(channel<result> f) override {
        /*
            REVISIT (sparent) : If we are in a final state then we should destruct the channel
            and not add it here.
        */
        {
        std::unique_lock<std::mutex> lock(_downstream_mutex);
        _downstream.emplace_back(f);
        }
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <typename> class receiver;

template <typename T>
class receiver {
    using ptr_t = std::shared_ptr<detail::shared_process_receiver<T>>;
    ptr_t _p;

    template <typename U>
    friend class channel;

    template <typename U>
    friend class receiver; // huh?

    receiver(ptr_t p) : _p(std::move(p)) { }

  public:
    receiver() = default;

    template <typename F>
    auto operator|(F&& f) const {
        auto p = std::make_shared<detail::shared_process<F>>(std::forward<F>(f), _p);
        _p->map(channel<argument_of_t<signature_t<F>>>(p));
        return receiver<result_of_t_<signature_t<F>>>(std::move(p));
    }
};

/**************************************************************************************************/

template <>
class receiver<void> {
    using ptr_t = std::shared_ptr<detail::shared_process_receiver<void>>;
    ptr_t _p;

    template <typename U>
    friend class channel;

    receiver(ptr_t p) : _p(std::move(p)) { }

  public:
    receiver() = default;
};

/**************************************************************************************************/

template <typename T>
class channel {
    using ptr_t = std::weak_ptr<detail::shared_process_channel<T>>;
    ptr_t _p;

    template <typename U>
    friend class receiver;

    channel(ptr_t p) : _p(std::move(p)) { }

  public:
    channel() = default;

    ~channel() {
        auto p = _p.lock();
        if (p) p->remove_channel();
    }

    channel(const channel& x) : _p(x._p) {
        auto p = _p.lock();
        if (p) p->add_channel();
    }

    channel(channel&&) noexcept = default;
    channel& operator=(const channel& x) {
        auto tmp = x; *this = std::move(tmp); return *this;
    }
    channel& operator=(channel&& x) noexcept = default;

    void close() {
        auto p = _p.lock();
        if (p) p->remove_channel();
        _p.reset();
    }

    template <typename... A>
    void operator()(A&&... args) const {
        auto p = _p.lock();
        if (p) p->send(std::forward<A>(args)...);
    }

    template <typename F>
    auto operator|(F&& f) {
        auto p = std::make_shared<detail::shared_process<F>>(std::forward<F>(f));
        _p = p;
        return receiver<result_of_t_<signature_t<F>>>(std::move(p));
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

    void close() { }

    R yield() { _done = true; return _bound(); }
    process_state state() const { return _done ? process_state::await : process_state::yield; }
};

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
