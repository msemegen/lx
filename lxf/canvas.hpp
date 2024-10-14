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
#include <lxf/common/scalar.hpp>
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
        Window(const device::Display& display_a)
            : handle(nullptr)
            , wnd_class(0x0u)
            , display(display_a)
        {
        }

    protected:
        HWND handle;
        ATOM wnd_class;

        device::Display display;

        VkSurfaceKHR vk_surface;
    };

public:
    enum class Size : common::Uint64
    {
        custom = 0x1ull,
        maximize
    };
    enum class Position : common::Uint64
    {
        custom = 0x1ull,
        centered,
        automatic
    };

    class Windowed : private Window
    {
    public:
        struct Descriptor
        {
            std::string_view title;

            Position position;
            Size size;
        };

        Windowed(const device::Display& display_a, const Descriptor& descriptor_a)
            : Window(display_a)
            , created(this->create_window(descriptor_a))
        {
        }

        void set_visible(bool visible_a);
        bool update();

        bool is_created() const
        {
            return this->created;
        }

        bool operator==(const Windowed& obj_a)
        {
            return this->handle == obj_a.handle && this->wnd_class == obj_a.wnd_class;
        }
        bool operator!=(const Windowed& obj_a)
        {
            return false == (*this == obj_a);
        }

    private:
        bool create_window(const Descriptor& descriptor_a);
        void destroy();

        bool created;

        friend class lxf::Windower;
    };
    class Fullscreen : private Window
    {
    public:
        struct Descriptor
        {
            common::Size<common::Uint32> resolution;
            common::Uint8 bits_per_pixel;
        };

        Fullscreen(const device::Display& display_a, const Descriptor descriptor_a)
            : Window(display_a)
        {
        }

        void set_descriptor(const Descriptor& descriptor_a);
        void set_visible(bool visible_a);

        bool update()
        {
            return true;
        }

        bool operator==(const Fullscreen& obj_a)
        {
            return this->handle == obj_a.handle && this->wnd_class == obj_a.wnd_class;
        }
        bool operator!=(const Fullscreen& obj_a)
        {
            return false == (*this == obj_a);
        }

    private:
        void destroy();

        friend class lxf::Windower;
    };
};

constexpr canvas::Size operator|(canvas::Size type_a, common::Size<common::Uint16> size_a)
{
    assert(canvas::Size::custom == type_a);

    return static_cast<canvas::Size>(static_cast<common::Uint16>(type_a) | (size_a.w << 16u) |
                                     (static_cast<common::Uint64>(size_a.h) << 32u));
}
constexpr canvas::Size operator&(canvas::Size left_a, canvas::Size right_a)
{
    return static_cast<canvas::Size>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}

constexpr canvas::Position operator|(canvas::Position type_a, common::Position<common::Uint16> size_a)
{
    assert(canvas::Position::custom == type_a);

    return static_cast<canvas::Position>(static_cast<common::Uint16>(type_a) | (size_a.x << 16u) |
                                         (static_cast<common::Uint64>(size_a.y) << 32u));
}
constexpr canvas::Position operator&(canvas::Position left_a, canvas::Position right_a)
{
    return static_cast<canvas::Position>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}
} // namespace lxf
