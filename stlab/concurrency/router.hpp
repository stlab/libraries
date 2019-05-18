/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_ROUTER_HPP
#define STLAB_CONCURRENCY_ROUTER_HPP

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <stlab/concurrency/channel.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

template <class T, class K>
class router {
    using channel_pair = std::pair<stlab::sender<T>, stlab::receiver<T>>;
    using route_pair = std::pair<K, channel_pair>;
    using routes = std::vector<route_pair>;

public:
    template <class E, class F>
    router(E executor, F router_func) : _self{std::make_shared<model<E, F>>(std::move(executor), std::move(router_func))} {}

    void set_ready() { _self->set_ready(); }
    stlab::receiver<T> get_route(K key) { return _self->get_route(std::move(key)); }
    stlab::receiver<T> get_default_route() { return _self->get_default_route(); }
    void operator()(T t) { _self->route(std::move(t)); }

private:
    struct concept_t {
        virtual stlab::receiver<T> get_route(K key) = 0;
        virtual stlab::receiver<T> get_default_route() = 0;
        virtual void set_ready() = 0;
        virtual void route(T t) = 0;
        virtual ~concept_t() = default;
    };

    template <class E, class F>
    struct model : concept_t {
        model(E executor, F router_func)
        : _executor{std::move(executor)}
        , _router_func{std::move(router_func)}
        , _sender_receiver{stlab::channel<T>(_executor)}
        {}

        void set_ready() override {
            std::sort(std::begin(_routes), std::end(_routes), [](const route_pair& a, const route_pair& b){
                return a.first < b.first;
            });
            for (auto& pair : _routes)
                pair.second.second.set_ready();
            _sender_receiver.second.set_ready();
        }

        stlab::receiver<T> get_default_route() override { return _sender_receiver.second; }

        stlab::receiver<T> get_route(K key) override {
            auto find_it = std::find_if(begin(_routes), std::end(_routes), [&](const route_pair& pair){
                return pair.first == key;
            });
            if (find_it != std::end(_routes)) return find_it->second.second;
            _routes.push_back(std::make_pair(std::move(key), stlab::channel<T>(_executor)));
            return _routes.back().second.second;
        }

        template<class C, typename std::enable_if_t<std::is_same<typename C::value_type, std::pair<K, bool>>::value, int> = 0>
        void route_keys(const C& key_set, T t) {
            bool did_route = false;
            auto find_it = std::begin(_routes);
            for (const auto& key : key_set) {
                if (!key.second) continue;
                find_it = std::lower_bound(find_it, std::end(_routes), key, [](const route_pair& a, const std::pair<K, bool>& k){
                    return a.first < k.first;
                });
                if (find_it == std::end(_routes)) break;
                if (find_it->first != key.first) continue;
                find_it->second.first(t);
                did_route = true;
            }
            if (!did_route) return _sender_receiver.first(std::move(t));
        }

        template<class C, typename std::enable_if_t<std::is_same<typename C::value_type, K>::value, int> = 0>
        void route_keys(const C& key_set, T t) {
            auto find_it = std::begin(_routes);
            for (const auto& key : key_set) {
                find_it = std::lower_bound(find_it, std::end(_routes), key, [](const route_pair& a, const K& k){
                    return a.first < k;
                });
                if (find_it == std::end(_routes)) return;
                if (find_it->first != key) continue;
                find_it->second.first(t);
            }
        }

        void route(T t) override {
            if (std::empty(_routes)) return _sender_receiver.first(std::move(t));
            const auto& key_set = _router_func(t);
            if (std::empty(key_set)) return _sender_receiver.first(std::move(t));
            route_keys(key_set, std::move(t)); 
        }

        E _executor;
        F _router_func;
        routes _routes;
        channel_pair _sender_receiver;
    };

    std::shared_ptr<concept_t> _self;
};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif
