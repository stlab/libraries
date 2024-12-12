/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_ENUM_OPS_HPP
#define STLAB_ENUM_OPS_HPP

/**************************************************************************************************/

#include <type_traits>

/**************************************************************************************************/

/*!
    \defgroup enum_ops Typesafe Integers and Bit Fields (enums)
    \ingroup utility

    \section Description Description

    \c enum_ops provides optional typesafe bitset and arithmetic operations for enumeration types.
    Without these typesafe operations, the compiler will promote the operand(s) to the appropriate
    integral type, and the result will be an integral type. When the typesafe operations have been
    defined for an enumeration type, \c E, the result will be of type \c E exactly when the
    operand(s) are of type \c E.

    \c auto stlab_enable_bitmask_enum(E) -> std::true_type;
    enables the bitset operations <code>~, |, &, ^, |=, &=, ^= </code>
    for enumeration type \c E.

    \c auto stlab_enable_arithmetic_enum(E) -> std::true_type;
    enables the typesafe arithmetic operations <code>+, -, *, /,
    %, +=, *=, -=, /=, \%=</code> for enumeration type \c E.

    \section Definition Definition

    Defined in \link enum_ops.hpp <code>stlab/enum_ops.hpp</code> \endlink

    \section Example Example

    The following is an example of code that will compile:
    \dontinclude enum_ops_example.cpp
    \skip start_of_example
    \until end_of_example

    The following is contains an example of code that will not compile
    since the typesafe operators have not been defined.

    \dontinclude enum_ops_example_fail.cpp
    \skip start_of_example
    \until end_of_example
*/

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

auto stlab_enable_bitmask_enum(...) -> std::false_type;
auto stlab_enable_arithmetic_enum(...) -> std::false_type;

/**************************************************************************************************/

namespace implementation {

/**************************************************************************************************/

template <class T>
using has_enabled_bitmask_t = decltype(stlab_enable_bitmask_enum(std::declval<T>()));

template <class T>
constexpr bool has_enabled_bitmask = has_enabled_bitmask_t<T>::value;

template <class T>
using has_enabled_arithmetic_t = decltype(stlab_enable_arithmetic_enum(std::declval<T>()));

template <class T>
constexpr bool has_enabled_arithmetic = has_enabled_arithmetic_t<T>::value;

template <class T, class U>
using enable_if_bitmask_or_arithmetic =
    std::enable_if_t<std::disjunction_v<stlab::implementation::has_enabled_bitmask_t<T>,
                                        stlab::implementation::has_enabled_arithmetic_t<T>>,
                     U>;

template <class, bool>
struct safe_underlying_type;

template <class T>
struct safe_underlying_type<T, true> {
    using type = std::underlying_type_t<T>;
};

template <class T>
struct safe_underlying_type<T, false> {
    using type = void;
};

template <class T>
using safe_underlying_type_t = typename safe_underlying_type<T, std::is_enum<T>::value>::type;

template <class U, class T>
using is_convertible_to_underlying =
    std::is_convertible<U, stlab::implementation::safe_underlying_type_t<T>>;

/**************************************************************************************************/

} // namespace implementation

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

template <class T>
constexpr auto operator&(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <class T>
constexpr auto operator~(T a)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(~static_cast<underlying>(a));
}

template <class T>
constexpr auto operator|(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <class T>
constexpr auto operator^(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <class T>
constexpr auto operator<<(T lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::make_unsigned<std::underlying_type_t<T>>;

    return lhs = static_cast<underlying>(lhs) << static_cast<underlying>(rhs);
}

template <class T>
constexpr auto operator>>(T lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T> {
    using underlying = std::make_unsigned<std::underlying_type_t<T>>;

    return lhs = static_cast<underlying>(lhs) >> static_cast<underlying>(rhs);
}

template <class T>
constexpr auto operator^=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs ^ rhs;
}

template <class T>
constexpr auto operator&=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs & rhs;
}

template <class T>
constexpr auto operator|=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs | rhs;
}

template <class T>
constexpr auto operator<<=(T& lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs << rhs;
}

template <class T>
constexpr auto operator>>=(T& lhs, std::size_t rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T&> {
    return lhs = lhs >> rhs;
}

template <class T, class U>
constexpr auto operator-(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) - static_cast<underlying>(rhs));
}

/**************************************************************************************************/

template <class T>
constexpr auto operator+(T a)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(+static_cast<underlying>(a));
}

template <class T>
constexpr auto operator-(T a)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(-static_cast<underlying>(a));
}

template <class T>
constexpr auto operator+(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) + static_cast<underlying>(rhs));
}

template <class T>
constexpr auto operator-(T lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) - static_cast<underlying>(rhs));
}

template <class T, class U>
constexpr auto operator*(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) * rhs);
}

template <class U, class T>
constexpr auto operator*(U lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(lhs * static_cast<underlying>(rhs));
}

template <class T, class U>
constexpr auto operator/(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) / rhs);
}

template <class T, class U>
constexpr auto operator%(T lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T> {
    using underlying = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<underlying>(lhs) % rhs);
}

template <class T>
constexpr auto operator+=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs = lhs + rhs;
}

template <class T>
constexpr auto operator-=(T& lhs, T rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs = lhs - rhs;
}

template <class T, class U>
constexpr auto operator*=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T&> {
    return lhs = lhs * rhs;
}

template <class T, class U>
constexpr auto operator/=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T&> {
    return lhs = lhs / rhs;
}

template <class T, class U>
constexpr auto operator%=(T& lhs, U rhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> &&
                            stlab::implementation::is_convertible_to_underlying<U, T>::value,
                        T&> {
    return lhs = lhs % rhs;
}

template <class T>
constexpr auto operator++(T& lhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs += static_cast<T>(1);
}

template <class T>
constexpr auto operator++(T& lhs, int)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    T result = lhs;
    lhs += static_cast<T>(1);
    return result;
}

template <class T>
constexpr auto operator--(T& lhs)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T&> {
    return lhs -= static_cast<T>(1);
}

template <class T>
constexpr auto operator--(T& lhs, int)
    -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T>, T> {
    T result = lhs;
    lhs -= static_cast<T>(1);
    return result;
}

/**************************************************************************************************/

template <class T>
constexpr auto operator==(T lhs, std::nullptr_t)
    -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool> {
    return !lhs;
}

template <class T>
constexpr auto operator==(std::nullptr_t, T rhs)
    -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool> {
    return !rhs;
}

template <class T>
constexpr auto operator!=(T lhs, std::nullptr_t rhs)
    -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool> {
    return !(lhs == rhs);
}

template <class T>
constexpr auto operator!=(std::nullptr_t lhs, T rhs)
    -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool> {
    return !(lhs == rhs);
}

template <class T>
constexpr auto operator!(T lhs) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool> {
    return !static_cast<bool>(lhs);
}

/**************************************************************************************************/

#endif

/**************************************************************************************************/
