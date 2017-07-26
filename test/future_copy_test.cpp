/*
    Copyright 2017 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

// stdc++
#include <iostream>

// boost
#include <boost/test/unit_test.hpp>

// stlab
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/test/model.hpp>

using namespace stlab;
using namespace std;

template<class T>
inline auto promise_future() {
    return package<T(T)>(immediate_executor, [](auto &&x) -> decltype(x) { return std::forward<decltype(x)>(x); });
}

BOOST_AUTO_TEST_CASE(future_operations_test) {
    annotate_counted::reset_operations_counter();
    const int expected_operations[enum_to_size_t(operations::last_entry)] = {1, 0, 4, 0, 0, 0, 5};

    {
        auto pack = promise_future<annotate_counted>();
        pack.first(annotate_counted());
        pack.second.then([](const annotate_counted &) {});
    }

    for (auto i = 0; i < enum_to_size_t(operations::last_entry); ++i) {
        BOOST_CHECK_EQUAL(expected_operations[i], annotate_counted::operations_counter[i]);
    }
}
