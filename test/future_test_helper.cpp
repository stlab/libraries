/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include "future_test_helper.hpp"

#include <utility>
#include <string>

namespace future_test_helper {

test_exception::test_exception(std::string  error) : _error(std::move(error)) {}

test_exception::test_exception(const char* error) : _error(error) {}

auto test_exception::what() const noexcept -> const char* { return _error.c_str(); }

} // namespace future_test_helper
