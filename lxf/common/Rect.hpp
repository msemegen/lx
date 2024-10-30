#pragma once

/*
 *   Name: Rect.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>

// lx
#include <lxf/common/Extent.hpp>
#include <lxf/common/Position.hpp>

namespace lxf::common {
template<typename Position_scalar_t, typename Size_scalar_t> struct Rect
{
    lxf::common::Position<Position_scalar_t> position;
    lxf::common::Extent<Size_scalar_t> size;
};
} // namespace lxf::common