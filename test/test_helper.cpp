/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include "test_helper.hpp"

namespace test_helper
{

test_exception::test_exception(const std::string &error)
        : _error(error) {}

test_exception::test_exception(const char *error)
        : _error(error) {}

const char *test_exception::what() const noexcept {
    return _error.c_str();
}

}