#pragma once

/*
 *   Name: Size.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>

// lx
#include <lxf/common/scalar.hpp>

namespace lxf {
namespace common {
template<typename Scalar_t> struct Size
{
    Scalar_t w = static_cast<Scalar_t>(0);
    Scalar_t h = static_cast<Scalar_t>(0);

    Uint32 operator[](Uint64 index_a) const
    {
        assert(index_a <= 2);

        return reinterpret_cast<const Uint32*>(this)[index_a];
    }

    Uint32& operator[](Uint64 index_a)
    {
        assert(index_a <= 2);

        return reinterpret_cast<Uint32*>(this)[index_a];
    }
};
} // namespace common
} // namespace lxf