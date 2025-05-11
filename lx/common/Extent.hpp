#pragma once

/*
 *	Name: Extent.hpp
 *  Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lx
#include <lx/common/non_constructible.hpp>

// std
#include <bit>
#include <cassert>
#include <cstddef>

namespace lx::common {
template<typename Scalar, std::size_t dimmensions> struct Extent : private common::non_constructible
{
};

template<typename Scalar> struct Extent<Scalar, 1u>
{
    Scalar w = static_cast<Scalar>(0);
    Scalar operator[](std::size_t index_a) const
    {
        assert(index_a < 1);
        return std::bit_cast<const Scalar*>(this)[index_a];
    }
    Scalar& operator[](std::size_t index_a)
    {
        assert(index_a < 1);
        return std::bit_cast<Scalar*>(this)[index_a];
    }
};

template<typename Scalar> struct Extent<Scalar, 2u>
{
    Scalar w = static_cast<Scalar>(0);
    Scalar h = static_cast<Scalar>(0);

    Scalar operator[](std::size_t index_a) const
    {
        assert(index_a < 2);
        return std::bit_cast<const Scalar*>(this)[index_a];
    }

    Scalar& operator[](std::size_t index_a)
    {
        assert(index_a < 2);
        return std::bit_cast<Scalar*>(this)[index_a];
    }
};

template<typename Scalar> struct Extent<Scalar, 3u>
{
    Scalar w = static_cast<Scalar>(0);
    Scalar h = static_cast<Scalar>(0);
    Scalar d = static_cast<Scalar>(0);

    Scalar operator[](std::size_t index_a) const
    {
        assert(index_a < 3);
        return std::bit_cast<const Scalar*>(this)[index_a];
    }

    Scalar& operator[](std::size_t index_a)
    {
        assert(index_a < 3);
        return std::bit_cast<Scalar*>(this)[index_a];
    }
};
} // namespace lx::common