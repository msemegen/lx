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
#include <lxf/common/scalar.hpp>

namespace lxf {
namespace common {
struct Version
{
    struct Components
    {
        common::Uint16 major;
        common::Uint16 minor;
        common::Uint16 path;
    };

    Version()
        : value(0x0u)
    {
    }

    Version(std::uint32_t combined_a)
        : value(combined_a)
    {
    }

    Version(Limited<Uint16, 0u, 0x3FFu> major_a, Limited<Uint16, 0u, 0x3FFu> minor_a, Limited<Uint16, 0u, 0xFFFu> patch_a)
    {
        this->value = (static_cast<common::Uint32>(major_a) << 22u) | (static_cast<common::Uint32>(minor_a) << 12U) |
                      (static_cast<common::Uint32>(patch_a));
    }

    void set(Limited<Uint16, 0u, 0x3FFu> major_a, Limited<Uint16, 0u, 0x3FFu> minor_a, Limited<Uint16, 0u, 0xFFFu> patch_a)
    {
        this->value = (static_cast<common::Uint32>(major_a) << 22u) | (static_cast<common::Uint32>(minor_a) << 12U) |
                      (static_cast<common::Uint32>(patch_a));
    }

    Components get() const
    {
        return { .major = static_cast<common::Uint16>((this->value >> 22u) & 0xFFFu),
                 .minor = static_cast<common::Uint16>((this->value >> 12u) & 0x3FFu),
                 .path = static_cast<common::Uint16>(this->value & 0xFFFu)

        };
    }

    operator common::Uint32() const
    {
        return this->value;
    }

    operator common::Uint32&()
    {
        return this->value;
    }

private:
    common::Uint32 value;
};
} // namespace common
} // namespace lxf