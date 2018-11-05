/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include "future_test_helper.hpp"

namespace future_test_helper {

test_exception::test_exception(const std::string& error) : _error(error) {}

test_exception::test_exception(const char* error) : _error(error) {}

const char* test_exception::what() const noexcept { return _error.c_str(); }

} // namespace future_test_helper

