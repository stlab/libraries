/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/**************************************************************************************************/

#ifndef TEST_HELPER_HPP_
#define TEST_HELPER_HPP_

namespace test_helper {
    
    class move_only_t {
    private:
        int _member;
    public:
        move_only_t(int member) : _member(member) {}
        move_only_t(const move_only_t&) = delete;
        move_only_t& operator=(const move_only_t&) = delete;
        move_only_t(move_only_t&&) = default;
        move_only_t& operator=(move_only_t&&) = default;
        virtual ~move_only_t() = default;
        
        int member() { return _member; }
    };
}

#endif
