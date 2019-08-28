/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/std_future_adaptor.hpp>
#include <stlab/concurrency/utility.hpp>

/**************************************************************************************************/

// I'm explicit about the types in this file because it's test/example code.

/**************************************************************************************************/

std::future<std::string> get_string_future() {
    return std::async([]() -> std::string {
        return "Hello, world!";
    });
}

std::future<void> get_void_future() {
    return std::async([] {
        // Do something important
    });
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(std_future_adaptor_test) {
    stlab::blocking_get(stlab::future_adaptor(get_string_future())
        .then([](std::string s) {
            std::cout << s << '\n';
        })
    );

    stlab::blocking_get(stlab::future_adaptor(get_string_future(), [](std::string s){
            return s.size();
        })
        .then([](std::size_t n) {
            std::cout << n << '\n';
        })
    );

    stlab::blocking_get(stlab::future_adaptor(get_void_future())
        .then([] {
            std::cout << "Hello, world!\n";
        })
    );
}

/**************************************************************************************************/
