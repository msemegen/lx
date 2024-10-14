#pragma once

/*
 *   Name: various.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <limits>
#include <type_traits>

// lx
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/scalar.hpp>

#undef max

namespace lxf {
namespace common {
struct various : private non_constructible
{
    template<typename Type> constexpr static Type get_enum_incorrect_value()
    {
        static_assert(true == std::is_enum<Type>::value);
        return static_cast<Type>(std::numeric_limits<std::underlying_type_t<Type>>::max());
    }

    template<typename Type, Uint64 count> constexpr static Uint64 countof(Type (&)[count])
    {
        return std::extent<Type[count]>::value;
    }
};
} // namespace common
} // namespace lxf
