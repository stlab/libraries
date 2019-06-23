/*
Copyright 2015 Adobe
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_ROUTER_HPP
#define STLAB_CONCURRENCY_ROUTER_HPP

#include <algorithm>
#include <utility>
#include <vector>

#include <stlab/concurrency/channel.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {
/**************************************************************************************************/

template <typename T>
using channel_t = std::pair<sender<T>, receiver<T>>;

namespace detail {

template <typename T, typename K, typename E, typename F>
class shared_router : public std::enable_shared_from_this<shared_router<T, K, E, F>> {
    using route_pair_t = std::pair<K, channel_t<T>>;
    E _executor; // of the router function
    F _router_function;
    std::vector<route_pair_t> _routes;
    bool _ready = false;

public:
    shared_router() = default;
    ~shared_router() = default;
    shared_router(shared_router&&) = default;
    shared_router(const shared_router&) = default;
    shared_router& operator=(shared_router&&) = default;
    shared_router& operator=(const shared_router&) = default;

    shared_router(E executor, F router_function) :
        _executor{executor}, _router_function(std::move(router_function)) {}

    template <typename... U>
    shared_router(E executor, F router_function, std::pair<K, channel_t<U>>... route_pairs) :
        _executor{std::move(executor)},
        _router_function(std::move(router_function)), _routes{
                                                          std::make_pair(route_pairs.first,
                                                                         route_pairs.second)...} {
        assert(std::is_sorted(_routes.cbegin(), _routes.cend(),
                              [](const auto& x, const auto& y) { return x.first < y.first; }));
        set_ready();
    }

    void set_ready() {
        for (auto& route : _routes)
            route.second.second.set_ready();
        _ready = true;
    }

    stlab::optional<receiver<T>> route(const K& key) const {
        auto find_it = std::lower_bound(_routes.cbegin(), _routes.cend(), key,
                                        [](const auto& p, const auto& k) { return p.first < k; });

        if (find_it == _routes.end() || find_it->first != key) return stlab::nullopt;
        return find_it->second.second;
    }

    template <typename Ex>
    receiver<T> add_route(K key, Ex executor) {
        assert(!_ready);

        channel_t<T> ch = channel<T>(executor);
        auto result = ch.second;
        auto insert_it = std::lower_bound(_routes.begin(), _routes.end(), key,
                                          [](const auto& p, const auto& k) { return p.first < k; });

        assert(insert_it == _routes.end() || insert_it->first != key);

        _routes.insert(insert_it, std::make_pair(key, std::move(ch)));

        return result;
    }

    receiver<T> add_route(K key) { return add_route(std::move(key), _executor); }

    void operator()(T arg) {
        assert(_ready);
        _executor([_weak_this = make_weak_ptr(this->shared_from_this()), _arg = std::move(arg)] {
            auto _this = _weak_this.lock();
            if (!_this) return;
            auto keys = _this->_router_function(_arg);
            auto find_it = std::begin(_this->_routes);
            for (const auto& key : keys) {
                find_it =
                    std::lower_bound(find_it, std::end(_this->_routes), key,
                                     [](const auto& p, const auto& k) { return p.first < k; });
                if (find_it == std::end(_this->_routes)) return;
                if (find_it->first != key) continue;
                find_it->second.first(_arg);
            }
        });
    }
};

} // namespace detail

/**************************************************************************************************/

template <typename T, typename K, typename E, typename F>
class router {
    using ptr_t = std::shared_ptr<detail::shared_router<T, K, E, F>>;
    ptr_t _p;

public:
    router() = default;
    ~router() = default;

    void swap(router& x) noexcept { std::swap(_p, x._p); }

    inline friend void swap(router& x, router& y) { x.swap(y); }
    inline friend bool operator==(const router& x, const router& y) {
        return (x._p && y._p && *x._p == *y._p) || (!x._p && !y._p);
    }
    inline friend bool operator!=(const router& x, const router& y) { return !(x == y); }

    bool valid() const { return static_cast<bool>(_p); }

    router(router&&) = default;
    router& operator=(router&&) = default;

    router(const router& x) : _p(std::make_shared<detail::shared_router<T, K, E, F>>(*x._p)) {}

    router& operator=(const router& x) {
        auto tmp = x;
        *this = std::move(tmp);
        return *this;
    }

    router(E executor, F router_function) :
        _p(std::make_shared<detail::shared_router<T, K, E, F>>(std::move(executor),
                                                               std::move(router_function))) {}

    template <typename... U>
    router(E executor, F router_function, std::pair<K, channel_t<U>>... route_pairs) :
        _p(std::make_shared<detail::shared_router<T, K, E, F>>(
            std::move(executor),
            std::move(router_function),
            std::make_pair(route_pairs.first, route_pairs.second)...)) {}

    void set_ready() { _p->set_ready(); }

    stlab::optional<receiver<T>> route(const K& key) const { return _p->route(key); }

    template <typename Ex>
    receiver<T> add_route(K key, Ex executor) {
        return _p->add_route(std::move(key), std::move(executor));
    }

    receiver<T> add_route(K key) { return _p->add_route(std::move(key)); }

    void operator()(T arg) { _p->operator()(std::move(arg)); }
};

template <typename T, typename K, typename E, typename F, typename... U>
auto make_router(E executor, F router_function, std::pair<K, channel_t<U>>... route_pairs) {
    return router<T, K, E, F>(std::move(executor), std::move(router_function), std::move(route_pairs)...);
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif