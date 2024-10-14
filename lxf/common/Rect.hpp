#pragma once

/*
 *   Name: Rect.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>

// lx
#include <lxf/common/Size.hpp>
#include <lxf/common/Position.hpp>
#include <lxf/common/scalar.hpp>

namespace lxf {
namespace common {
template<typename Position_scalar_t, typename Size_scalar_t> struct Rect
{
    Position<Position_scalar_t> position;
    Size<Size_scalar_t> size;
};
} // namespace common
} // namespace lxf