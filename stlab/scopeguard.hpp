/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

Scope Guard based on the idea from Andrei Alexandrescu
*/

#pragma once

#define CONCATENATE_IMPL(s1,s2) s1##s2
#define CONCATENATE(s1,s2) CONCATENATE_IMPL(s1,s2)

// Some compiler does not support __COUNTER__ so there is an alternative
// implementation with __LINE__
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str)\
  CONCATENATE(str,__COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
  CONCATENATE(str,__LINE__)
#endif

namespace stlab
{
  namespace detail
  {
    template <class Functor>
    class scope_guard
    {
    public:
      scope_guard(Functor fn)
        : _f(std::move(fn))
        , _active(true)
      {}

      scope_guard(scope_guard&& rhs)
        : _f(std::move(rhs._f))
        , _active(rhs._active)
      {
        rhs._active = false;
      }

      ~scope_guard() {
        if (_active)
            _f();
      }

      void dismiss() {
        _active = false;
      }

    private:
      Functor _f;
      bool _active;

      scope_guard() = delete;
      scope_guard(const scope_guard&) = delete;
      scope_guard& operator=(const scope_guard&) = delete;
    };

    enum class scope_guard_on_exit {};
    
    template <class Functor>
    scope_guard<Functor> operator+(scope_guard_on_exit, Functor&& fn)
    {
      return scope_guard<Functor>(std::forward<Functor>(fn));
    }
  }
}

/**
 * Use this macro in case of an anonymous scope guard like
 * SCOPE_EXIT { foo.needsToBeDoneOnLeavingTheScope(); };
 * 
 * Note: Don't forget the semicolon after the closing brace!
 */
#define SCOPE_EXIT \
  auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
  = ::stlab::detail::scope_guard_on_exit() + [&]()


/**
* Use this macro in case you like an explicit scope_guard
* This is handy if there is a case where you want to dismiss the 
* action to be taken on leaving the scope.
* 
* auto localGuard = EXPLICIT_SCOPE_EXIT { foo.needsToBeDoneOnLeavingTheScope(); };
* ...
* if (specialCase)
* {
*   localGuard.dismiss();
* }
* 
* Note: Don't forget the semicolon after the closing brace!
*/

#define EXPLICIT_SCOPE_EXIT \
  ::stlab::detail::scope_guard_on_exit() + [&]()
