
/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_MODULE stlab_libraries_tests
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include <stlab/pre_exit.hpp>

int main(int argc, char* argv[])
{
  int result = ::boost::unit_test::unit_test_main(init_unit_test, argc, argv);
  stlab::pre_exit();
  return result;
}
