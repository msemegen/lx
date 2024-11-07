#pragma once

/*
 *   Name: Position.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <bit>
#include <cassert>
#include <cstddef>

// lxf
#include <lxf/common/non_constructible.hpp>

namespace lxf::common {
template<typename Scalar, std::size_t dimmensions> struct Position : private common::non_constructible
{
};
template<typename Scalar> struct Position<Scalar, 2u>
{
    Scalar x = static_cast<Scalar>(0);
    Scalar y = static_cast<Scalar>(0);

    Scalar operator[](std::size_t index) const
    {
        assert(index <= 2);

        return std::bit_cast<const Scalar*>(this)[index];
    }

    Scalar& operator[](std::size_t index)
    {
        assert(index <= 2);

        return std::bit_cast<Scalar*>(this)[index];
    }
};
} // namespace lxf::common