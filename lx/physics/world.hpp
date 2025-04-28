#pragma once

// lx
#include <lx/common/non_constructible.hpp>

// std
#include <cstddef>

namespace lx::physics {
struct world : private lx::common::non_constructible
{
    using Id = std::size_t;

    struct Properties
    {
    };

    Id create(const Properties&)
    {
        return {};
    }
};
} // namespace lx::physics