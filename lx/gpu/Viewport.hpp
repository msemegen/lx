#pragma once

// lx
#include <lx/common/Extent.hpp>
#include <lx/common/Position.hpp>

namespace lx::gpu {
template<typename Type> struct Viewport
{
    lx::common::Position<Type, 2u> position;
    lx::common::Extent<Type, 2u> size;

    Type min_depth;
    Type max_depth;
};
} // namespace lx::gpu