#pragma once

// lx
#include <lx/common/non_constructible.hpp>

// std
#include <bit>
#include <cassert>
#include <cstddef>

namespace lx::common {
template<typename Scalar, std::size_t dimmensions> struct Position : private common::non_constructible
{
};

template<typename Scalar> struct Position<Scalar, 2u>
{
    Scalar x = static_cast<Scalar>(0);
    Scalar y = static_cast<Scalar>(0);

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

template<typename Scalar> struct Position<Scalar, 3u>
{
    Scalar x = static_cast<Scalar>(0);
    Scalar y = static_cast<Scalar>(0);
    Scalar z = static_cast<Scalar>(0);

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