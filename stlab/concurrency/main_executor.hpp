/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP
#define STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP

#ifdef STLAB_MAIN_EXECUTOR_QT
#include <QCoreApplication>
#include <QEvent>
#include <stlab/concurrency/task.hpp>
#include <memory>
#elif defined(STLAB_MAIN_EXECUTOR_LIBDISPATCH)
#include <dispatch/dispatch.h>
#elif defined(STLAB_MAIN_EXECUTOR_EMSCRIPTEN)
#include <stlab/concurrency/default_executor.hpp>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

#ifdef STLAB_MAIN_EXECUTOR_QT

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

#elif defined(STLAB_MAIN_EXECUTOR_LIBDISPATCH)

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

#elif defined(STLAB_MAIN_EXECUTOR_EMSCRIPTEN

using main_executor_type = default_executor_type;

#endif // STLAB_MAIN_EXECUTOR_QT

} // namespace detail

#ifndef STLAB_MAIN_EXECUTOR_NONE
constexpr auto main_executor = detail::main_executor_type{};
#endif

} // namespace v1

} // namespace stlab

#endif // STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP
