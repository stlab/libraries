/*
    Copyright 2022 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

// header file included first to ensure it stands alone
#include <stlab/utility.hpp>

#include <type_traits>

// boost
#include <boost/test/unit_test.hpp>
#include <stlab/test/model.hpp>

/**************************************************************************************************/

namespace {

/**************************************************************************************************/

// revisit - this should go into the library, but we're not certain of the form
template <class T, class R>
using enable_if_rvalue_reference_t = std::enable_if_t<std::is_rvalue_reference<T>::value, R>;

// a test function that only excepts rvalue references
template <class T>
auto sink_only(T&& a) -> enable_if_rvalue_reference_t<T&&, T> {
    return std::move(a);
}

/**************************************************************************************************/

} // namespace

/**************************************************************************************************/

using namespace std;
using namespace stlab;

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(copy_test) {
    annotate_counters counters;
    annotate a{counters};
    a = sink_only(std::move(a));
    BOOST_CHECK(counters._copy_ctor == 0);
    BOOST_CHECK(counters._copy_assign_lhs == 0);
    BOOST_CHECK(counters._copy_assign_rhs == 0);
    a = sink_only(stlab::copy(a));
    BOOST_CHECK(counters._copy_ctor == 1);
    BOOST_CHECK(counters._copy_assign_lhs == 0);
    BOOST_CHECK(counters._copy_assign_rhs == 0);
}

/**************************************************************************************************/
