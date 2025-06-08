#pragma once

/*
 *	Name: Display.hpp
 *  Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lx
#include <lx/common/Rect.hpp>
#include <lx/containers/String.hpp>

// win32
#include <Windows.h>

// std
#include <algorithm>
#include <cstdint>
#include <string_view>

namespace lx::devices {
struct Display
{
    enum class Kind : std::uint32_t
    {
        primary,
        additional
    };

    using enum Kind;

    const Kind kind = {};
    const common::Rect<std::int64_t, std::uint64_t> logical_rect = {};
    const common::Rect<std::int64_t, std::uint64_t> physical_rect = {};
    const std::uint8_t bits_per_pixel = 0u;

    const lx::containers::String<char, CCHDEVICENAME> name;

    Display() = default;
    Display(const Display&) = default;
    Display(Display&&) = default;

    Display& operator=(const Display&)
    {
        return *this;
    }

    Display(HMONITOR handle_a,
            bool is_primary_a,
            std::uint8_t bits_per_pixel_a,
            std::string_view name_a,
            const common::Rect<std::int64_t, std::uint64_t>& logical_rect_a,
            const common::Rect<std::int64_t, std::uint64_t>& physical_rect_a)
        : handle(handle_a)
        , kind(true == is_primary_a ? Kind::primary : Kind::additional)
        , logical_rect(logical_rect_a)
        , physical_rect(physical_rect_a)
        , bits_per_pixel(bits_per_pixel_a)
        , name(name_a)
    {
    }

    operator HMONITOR() const
    {
        return this->handle;
    }
    operator HMONITOR()
    {
        return this->handle;
    }

private:
    HMONITOR handle = nullptr;
};
} // namespace lx::devices