/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <stlab/pre_exit.hpp>

#define BOOST_TEST_MODULE stlab_libraries_tests
#include <boost/test/unit_test.hpp>

namespace stlab_test {

struct pre_exit_fixture {
    void teardown() { stlab::pre_exit(); }
};

BOOST_TEST_GLOBAL_FIXTURE(pre_exit_fixture);

} // namespace stlab_test
