#pragma once

// lx
#include <lx/common/Extent.hpp>
#include <lx/common/Position.hpp>

namespace lx::gpu {
struct Viewport
{
    lx::common::Position<std::int32_t, 2u> position;
    lx::common::Extent<std::uint32_t, 2u> size;

    float min_depth;
    float max_depth;
};
} // namespace lx::gpu