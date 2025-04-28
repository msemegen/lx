#pragma once

// lx
#include <lx/common/Extent.hpp>
#include <lx/common/Position.hpp>

namespace lx::common {
template<typename Position_scalar, typename Extent_scalar> struct Rect
{
    common::Position<Position_scalar, 2u> position;
    common::Extent<Extent_scalar, 2u> size;
};
} // namespace lx::common