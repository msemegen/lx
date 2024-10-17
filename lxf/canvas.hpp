#pragma once

/*
 *   Name: Canvas.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <string>
#include <string_view>

// platform
#include <Windows.h>
#include <vulkan/vulkan.h>

// lxf
#include <lxf/common/Position.hpp>
#include <lxf/common/Size.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/device.hpp>

namespace lxf {
class Windower;
}

namespace lxf {
class canvas : private common::non_constructible
{
private:
    class Window : private common::non_copyable
    {
    public:
        Window()
            : created(false)
            , handle(nullptr)
            , wnd_class(0x0u)
        {
        }

        [[nodiscard]] bool is_created() const
        {
            return this->created;
        }

    protected:
        bool created;

        HWND handle;
        ATOM wnd_class;

        VkSurfaceKHR vk_surface;
    };

public:
    enum class Size : std::uint64_t
    {
        custom = 0x1ull,
        maximize
    };
    enum class Position : std::uint64_t
    {
        custom = 0x1ull,
        centered,
        automatic
    };

    class Windowed : public Window
    {
    public:
        struct Descriptor
        {
            std::string_view title;

            Position position;
            Size size;
        };

        Windowed(const device::Display* p_display_a, const Descriptor& descriptor_a)
            : Window()
        {
            this->created = this->create_window(p_display_a, descriptor_a);
        }

        void set_visible(bool visible_a);
        bool update();

        bool operator==(const Windowed& obj_a)
        {
            return this->handle == obj_a.handle && this->wnd_class == obj_a.wnd_class;
        }
        bool operator!=(const Windowed& obj_a)
        {
            return false == (*this == obj_a);
        }

    private:
        bool create_window(const device::Display* p_display_a, const Descriptor& descriptor_a);
        void destroy();

        friend class lxf::Windower;
    };
    class Fullscreen : public Window
    {
    public:
        struct Descriptor
        {
            common::Size<std::uint32_t> resolution;
            std::uint8_t bits_per_pixel;
        };

        Fullscreen(const device::Display* p_display_a, const Descriptor descriptor_a)
            : Window()
        {
            this->created = this->create_window(p_display_a, descriptor_a);
        }

        void set_visible(bool visible_a);
        bool update();

        bool operator==(const Fullscreen& obj_a)
        {
            return this->handle == obj_a.handle && this->wnd_class == obj_a.wnd_class;
        }
        bool operator!=(const Fullscreen& obj_a)
        {
            return false == (*this == obj_a);
        }

    private:
        bool create_window(const device::Display* p_display_a, const Descriptor& descriptor_a);
        void destroy();

        friend class lxf::Windower;
    };
};

constexpr canvas::Size operator|(canvas::Size type_a, common::Size<std::uint16_t> size_a)
{
    assert(canvas::Size::custom == type_a);

    return static_cast<canvas::Size>(static_cast<std::uint16_t>(type_a) | (size_a.w << 16u) |
                                     (static_cast<std::uint64_t>(size_a.h) << 32u));
}
constexpr canvas::Size operator&(canvas::Size left_a, canvas::Size right_a)
{
    return static_cast<canvas::Size>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}

constexpr canvas::Position operator|(canvas::Position type_a, common::Position<std::uint16_t> size_a)
{
    assert(canvas::Position::custom == type_a);

    return static_cast<canvas::Position>(static_cast<std::uint16_t>(type_a) | (size_a.x << 16u) |
                                         (static_cast<std::uint64_t>(size_a.y) << 32u));
}
constexpr canvas::Position operator&(canvas::Position left_a, canvas::Position right_a)
{
    return static_cast<canvas::Position>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}
} // namespace lxf
