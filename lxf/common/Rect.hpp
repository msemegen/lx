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
template<typename Position_scalar, typename Extent_scalar> struct Rect
{
    lxf::common::Position<Position_scalar, 2u> position;
    lxf::common::Extent<Extent_scalar, 2u> size;
};
} // namespace lxf::common