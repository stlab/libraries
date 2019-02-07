/*
    Copyright 2017 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

// stdc++
#include <iostream>
#include <thread>

// boost
#include <boost/test/unit_test.hpp>

// stlab
#ifndef STLAB_DISABLE_FUTURE_COROUTINES
#define STLAB_DISABLE_FUTURE_COROUTINES
#endif

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/tuple_algorithm.hpp>
#include <stlab/concurrency/utility.hpp>

/**************************************************************************************************/

namespace {

/**************************************************************************************************/

template <typename T>
std::string demangle() {
#if defined(__clang__)
    struct freer_t {
        void operator()(void* x) const { std::free(x); }
    };
    std::unique_ptr<char, freer_t> c_str(
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr));
    return c_str.get();
#else
    return std::string();
#endif
}

/**************************************************************************************************/

} // namespace

/**************************************************************************************************/

using namespace std;
using namespace stlab;

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(remove_placeholder_test) {
    auto x = std::make_tuple(10, detail::placeholder(), 25.0, detail::placeholder());

    detail::apply_indexed<
        index_sequence_transform_t<make_index_sequence<tuple_size<decltype(x)>::value>,
                                   detail::remove_placeholder<decltype(x)>::function>>(
        [](auto... args) { for_each_argument([](auto x) { cout << x << endl; }, args...); }, x);
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(add_placeholder_test) {
    using interim_t = placeholder_tuple<int, void, int, void>;

    auto x = interim_t(10, detail::placeholder(), 25, detail::placeholder());

    detail::apply_indexed<
        index_sequence_transform_t<make_index_sequence<tuple_size<decltype(x)>::value>,
                                   detail::remove_placeholder<decltype(x)>::function>>(
        [](auto... args) { for_each_argument([](auto x) { cout << x << endl; }, args...); }, x);
}

/**************************************************************************************************/

template <typename F, typename... Ts>
void when_all_typecheck(F, future<Ts>...) {
    using pt_t = placeholder_tuple<Ts...>;
    using opt_t = optional_placeholder_tuple<Ts...>;
    using vt_t = voidless_tuple<Ts...>;
    using result_t = decltype(detail::apply_tuple(std::declval<F>(), std::declval<vt_t>()));

    cout << "pt: " << demangle<pt_t>() << "\n";
    cout << "opt_t: " << demangle<opt_t>() << "\n";
    cout << "vt_t: " << demangle<vt_t>() << "\n";
    cout << "result_t: " << demangle<result_t>() << "\n";
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_typecheck_test) {
    auto fv = [] { return stlab::make_ready_future(stlab::default_executor); };
    auto fi = [] {
        static int count_s{0};
        return stlab::make_ready_future<int>(count_s++, stlab::default_executor);
    };

    when_all_typecheck(
        [](auto... args) { for_each_argument([](auto x) { cout << x << endl; }, args...); }, fv(),
        fi(), fv(), fi());
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_when_all_int_int) {
    auto fi = [] {
        static int count_s{0};
        return stlab::make_ready_future<int>(count_s++, stlab::default_executor);
    };

    auto f = when_all(
        stlab::default_executor,
        [](auto... args) { for_each_argument([](auto x) { cout << x << "\n"; }, args...); }, fi(),
        fi());

    while (!f.get_try())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_when_all_void) {
    auto fv = [] { return stlab::make_ready_future(stlab::default_executor); };

    auto f = when_all(stlab::default_executor, []() { cout << "done!\n"; }, fv());

    while (!f.get_try())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_when_all_void_int) {
    auto fv = [] { return stlab::make_ready_future(stlab::default_executor); };
    auto fi = [] {
        static int count_s{0};
        return stlab::make_ready_future<int>(count_s++, stlab::default_executor);
    };

    auto f = when_all(
        stlab::default_executor,
        [](auto... args) { for_each_argument([](auto x) { cout << x << "\n"; }, args...); }, fv(),
        fi());

    while (!f.get_try())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_when_all_int_void) {
    auto fv = [] { return stlab::make_ready_future(stlab::default_executor); };
    auto fi = [] {
        static int count_s{0};
        return stlab::make_ready_future<int>(count_s++, stlab::default_executor);
    };

    auto f = when_all(
        stlab::default_executor,
        [](auto... args) { for_each_argument([](auto x) { cout << x << "\n"; }, args...); }, fi(),
        fv());

    while (!f.get_try())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_when_all_int_void_string_void_bool_void) {
    auto fv = [] { return stlab::make_ready_future(stlab::default_executor); };
    auto fi = [] { return stlab::make_ready_future<int>(42, stlab::default_executor); };
    auto fs = [] {
        return stlab::make_ready_future<std::string>("Hello, world!", stlab::default_executor);
    };
    auto fb = [] { return stlab::make_ready_future<bool>(true, stlab::default_executor); };

    auto f = when_all(
        stlab::default_executor,
        [](auto... args) { for_each_argument([](auto x) { cout << x << "\n"; }, args...); }, fi(),
        fv(), fs(), fv(), fb(), fv());

    while (!f.get_try())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(future_when_any_void) {
    auto fv = [] { return stlab::make_ready_future(stlab::default_executor); };

    auto f =
        when_any(stlab::default_executor, [](size_t index) { std::cout << "f: " << index << '\n'; },
                 fv(), fv(), fv(), fv(), fv(), fv());

    while (!f.get_try())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**************************************************************************************************/
