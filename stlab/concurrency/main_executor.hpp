/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP
#define STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP

#include <utility>

#include <stlab/config.hpp>

#if STLAB_MAIN_EXECUTOR(QT)
#include <QCoreApplication>
#include <QEvent>
#include <stlab/concurrency/task.hpp>
#include <memory>
#elif STLAB_MAIN_EXECUTOR(LIBDISPATCH)
#include <dispatch/dispatch.h>
#elif STLAB_MAIN_EXECUTOR(EMSCRIPTEN)
#include <stlab/concurrency/default_executor.hpp>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

#if STLAB_MAIN_EXECUTOR(QT)

class main_executor_type {
    using result_type = void;

    struct event_receiver;

    class executor_event : public QEvent {
        stlab::task<void()> _f;
        std::unique_ptr<event_receiver> _receiver;

    public:
        executor_event() : QEvent(QEvent::User), _receiver(new event_receiver()) {
            _receiver->moveToThread(QCoreApplication::instance()->thread());
        }

        template <typename F>
        void set_task(F&& f) {
            _f = std::forward<F>(f);
        }

        void execute() { _f(); }

        QObject* receiver() const { return _receiver.get(); }
    };

    struct event_receiver : public QObject {
        bool event(QEvent* event) override {
            auto myEvent = dynamic_cast<executor_event*>(event);
            if (myEvent) {
                myEvent->execute();
                return true;
            }
            return false;
        }
    };

public:
    template <typename F>
    void operator()(F f) const {
        auto event = std::make_unique<executor_event>();
        event->set_task(std::move(f));
        auto receiver = event->receiver();
        QCoreApplication::postEvent(receiver, event.release());
    }
};

/**************************************************************************************************/

#elif STLAB_MAIN_EXECUTOR(LIBDISPATCH)

struct main_executor_type {
    using result_type = void;

    template <typename F>
    void operator()(F f) const {
        using f_t = decltype(f);

        dispatch_async_f(dispatch_get_main_queue(), new f_t(std::move(f)), [](void* f_) {
            auto f = static_cast<f_t*>(f_);
            (*f)();
            delete f;
        });
    }
};

#elif STLAB_MAIN_EXECUTOR(EMSCRIPTEN)

struct main_scheduler_type {
    using result_type = void;

    template <class F>
    void operator()(F&& f) const {
        using function_type = typename std::remove_reference<F>::type;
        auto p = new function_type(std::forward<F>(f));

         /*
           `emscripten_async_run_in_main_runtime_thread()` schedules a function to run on the main
            JS thread, however, the code can be executed at any POSIX thread cancelation point if
            wasm code is executing on the JS main thread.
            Executing the code from a POSIX thread cancelation point can cause problems, including
            deadlocks and data corruption. Consider:
            ```
                mutex.lock();   // <-- If reentered, would deadlock here
                new T;          // <-- POSIX cancelation point, could reenter
            ```
            The call to `emscripten_async_call()` bounces the call to execute as part of the main
            run-loop on the current (main) thread. This avoids nasty reentrancy issues if executed
            from a POSIX thread cancelation point.
        */

        emscripten_async_run_in_main_runtime_thread(
            EM_FUNC_SIG_VI,
            static_cast<void(*)(void*)>([](void* f_) {
                emscripten_async_call(
                    [](void* f_) {
                        auto f = static_cast<function_type*>(f_);
                        // Note the absence of exception handling.
                        // Operations queued to the task system cannot throw as a precondition. 
                        // We use packaged tasks to marshal exceptions.
                        (*f)();
                        delete f;
                    },
                    f_, 0);
            }),
            p);
    }
};

#elif STLAB_MAIN_EXECUTOR(NONE)

// For documentation only
struct main_executor_type {
    using result_type = void;

    template <typename F>
    void operator()(F f) const { }
};

#endif

} // namespace detail

constexpr auto main_executor = detail::main_executor_type{};

} // namespace v1

} // namespace stlab

#endif // STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP
