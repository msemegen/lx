#pragma once

/*
 *   Name: Size.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <cstdint>

namespace lxf::common {
template<typename Scalar_t> struct Size
{
    Scalar_t w = static_cast<Scalar_t>(0);
    Scalar_t h = static_cast<Scalar_t>(0);

    Scalar_t operator[](std::size_t index_a) const
    {
        assert(index_a <= 2);

        return reinterpret_cast<const Scalar_t*>(this)[index_a];
    }

    Scalar_t& operator[](std::size_t index_a)
    {
        assert(index_a <= 2);

        return reinterpret_cast<Scalar_t*>(this)[index_a];
    }
};
} // namespace lxf::common