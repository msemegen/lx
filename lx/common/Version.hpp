#pragma once

/*
 *   Name: Version.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lx
#include <lx/common/ranged.hpp>

// std
#include <cstdint>
#include <utility>

namespace lx::common {

/// @brief A struct for managing version numbers.
struct Version
{
    struct Components
    {
        std::uint16_t major;
        std::uint16_t minor;
        std::uint16_t patch;
    };

    Version()
        : value(0x0u)
    {
    }

    Version(std::uint32_t combined)
        : value(combined)
    {
    }

    constexpr Version(const Components& components)
    {
        this->value = (static_cast<std::uint32_t>(components.major) << 22u) | (static_cast<std::uint32_t>(components.minor) << 12u) |
                      (static_cast<std::uint32_t>(components.patch));
    }

    Version(ranged<std::uint16_t, 0u, 0x3FFu> major, ranged<std::uint16_t, 0u, 0x3FFu> minor, ranged<std::uint16_t, 0u, 0xFFFu> patch)
    {
        this->value =
            (static_cast<std::uint32_t>(major) << 22u) | (static_cast<std::uint32_t>(minor) << 12U) | (static_cast<std::uint32_t>(patch));
    }

    void set(ranged<std::uint16_t, 0u, 0x3FFu> major, ranged<std::uint16_t, 0u, 0x3FFu> minor, ranged<std::uint16_t, 0u, 0xFFFu> patch)
    {
        this->value =
            (static_cast<std::uint32_t>(major) << 22u) | (static_cast<std::uint32_t>(minor) << 12U) | (static_cast<std::uint32_t>(patch));
    }

    constexpr operator Components() const
    {
        return { .major = static_cast<std::uint16_t>((this->value >> 22u) & 0xFFFu),
                 .minor = static_cast<std::uint16_t>((this->value >> 12u) & 0x3FFu),
                 .patch = static_cast<std::uint16_t>(this->value & 0xFFFu)

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
} // namespace lx::common