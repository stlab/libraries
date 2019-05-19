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
#include <stdexcept>

#include <stlab/concurrency/channel.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

template <class T, class K>
class router {
    using channel_t = std::pair<sender<T>, receiver<T>>;
    using route_pair = std::pair<const K, channel_t>;
    using routes = std::vector<route_pair>;

public:
    template <class I, class F>
    router(std::pair<I, I> key_range, F router_func) : _self{std::make_unique<model<F>>(make_routes(key_range), std::move(router_func))} {}

    void set_ready() { _self->set_ready(); }
    receiver<T> get_route(const K& key) const { return _self->get_route(key); }
    void operator()(T t) { _self->route(std::move(t)); }

private:
    template<class I>
    static routes make_routes(const std::pair<I, I>& range) {
        routes result;
        std::transform(range.first, range.second, std::back_inserter(result), [](auto route_key) -> route_pair {
            return {std::get<0>(route_key), channel<T>(std::get<1>(route_key))};
        });
        if (std::empty(result)) throw std::runtime_error("no routes provided");
        return result;
    }

    struct concept_t {
        virtual receiver<T> get_route(const K& key) const = 0;
        virtual void set_ready() = 0;
        virtual void route(T t) = 0;
        virtual ~concept_t() = default;
    };

    template <class F>
    struct model : concept_t {
        model(routes routes, F router_func)
        : _routes{std::move(routes)}
        , _router_func{std::move(router_func)}
        {}

        void set_ready() override {
            for (auto& pair : _routes) pair.second.second.set_ready();
        }

        receiver<T> get_route(const K& key) const override {
            auto find_it = std::find_if(begin(_routes), std::end(_routes), [&](const route_pair& pair){
                return pair.first == key;
            });
            if (find_it != std::end(_routes)) return find_it->second.second;
            throw std::runtime_error("no such route");
        }

        void route(T t) override {
            const auto& keys = _router_func(t);
            if (std::empty(keys)) return;
            route(keys, std::move(t));
        }

        template<class C, typename std::enable_if_t<std::is_same<typename C::value_type, std::pair<K, bool>>::value, int> = 0>
        void route(const C& keys, T t) {
            auto find_it = std::begin(_routes);
            for (const auto& key : keys) {
                if (!key.second) continue;
                find_it = std::lower_bound(find_it, std::end(_routes), key, [](const route_pair& a, const std::pair<K, bool>& k){
                    return a.first < k.first;
                });
                if (find_it == std::end(_routes)) break;
                if (find_it->first != key.first) continue;
                find_it->second.first(t);
            }
        }

        template<class C, typename std::enable_if_t<std::is_same<typename C::value_type, K>::value, int> = 0>
        void route(const C& keys, T t) {
            auto find_it = std::begin(_routes);
            for (const auto& key : keys) {
                find_it = std::lower_bound(find_it, std::end(_routes), key, [](const route_pair& a, const K& k){
                    return a.first < k;
                });
                if (find_it == std::end(_routes)) return;
                if (find_it->first != key) continue;
                find_it->second.first(t);
            }
        }

        F _router_func;
        routes _routes;
    };

    std::unique_ptr<concept_t> _self;
};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif
