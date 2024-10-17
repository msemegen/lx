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

#undef max

namespace lxf::common {
struct various : private non_constructible
{
    template<typename Type> constexpr static Type get_enum_incorrect_value()
    {
        static_assert(true == std::is_enum<Type>::value);
        return static_cast<Type>(std::numeric_limits<std::underlying_type_t<Type>>::max());
    }

    template<typename Type, std::uint64_t count> constexpr static std::uint64_t countof(Type (&)[count])
    {
        return std::extent<Type[count]>::value;
    }
};
} // namespace lxf::common