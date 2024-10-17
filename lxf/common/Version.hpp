#pragma once

/*
 *   Name: Version.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cstdint>
#include <utility>

// lx
#include <lxf/common/Limited.hpp>

namespace lxf::common {
struct Version
{
    struct Components
    {
        std::uint16_t major;
        std::uint16_t minor;
        std::uint16_t path;
    };

    Version()
        : value(0x0u)
    {
    }

    Version(std::uint32_t combined_a)
        : value(combined_a)
    {
    }

    Version(Limited<std::uint16_t, 0u, 0x3FFu> major_a,
            Limited<std::uint16_t, 0u, 0x3FFu> minor_a,
            Limited<std::uint16_t, 0u, 0xFFFu> patch_a)
    {
        this->value = (static_cast<std::uint32_t>(major_a) << 22u) | (static_cast<std::uint32_t>(minor_a) << 12U) |
                      (static_cast<std::uint32_t>(patch_a));
    }

    void
    set(Limited<std::uint16_t, 0u, 0x3FFu> major_a, Limited<std::uint16_t, 0u, 0x3FFu> minor_a, Limited<std::uint16_t, 0u, 0xFFFu> patch_a)
    {
        this->value = (static_cast<std::uint32_t>(major_a) << 22u) | (static_cast<std::uint32_t>(minor_a) << 12U) |
                      (static_cast<std::uint32_t>(patch_a));
    }

    Components get() const
    {
        return { .major = static_cast<std::uint16_t>((this->value >> 22u) & 0xFFFu),
                 .minor = static_cast<std::uint16_t>((this->value >> 12u) & 0x3FFu),
                 .path = static_cast<std::uint16_t>(this->value & 0xFFFu)

        };
    }

    operator std::uint32_t() const
    {
        return this->value;
    }

    operator std::uint32_t&()
    {
        return this->value;
    }

private:
    std::uint32_t value;
};
} // namespace lxf::common