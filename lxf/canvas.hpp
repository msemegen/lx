#pragma once

/*
 *   Name: Canvas.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <string>
#include <string_view>

// lxf
#include <lxf/common/Position.hpp>
#include <lxf/common/Rect.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/loader/vulkan.hpp>

namespace lxf {
class Windower;
} // namespace lxf

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

        operator HWND() const
        {
            return this->handle;
        }
        operator VkSurfaceKHR() const
        {
            return this->vk_surface;
        }

    protected:
        bool created;

        HWND handle;
        VkSurfaceKHR vk_surface;
        ATOM wnd_class;
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
        struct Properties
        {
            std::string_view title;

            Position position;
            Size size;
        };

        Windowed(common::Rect<std::int32_t, std::uint32_t> screen_a, const Properties& properties_a)
            : Window()
        {
            this->created = this->create_window(screen_a, properties_a);
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
        bool create_window(common::Rect<std::int32_t, std::uint32_t> screen_a, const Properties& properties_a);
        void destroy();

        friend class lxf::Windower;
    };
    class Fullscreen : public Window
    {
    public:
        struct Properties
        {
            common::Extent<std::uint32_t> resolution;
            std::uint8_t bits_per_pixel;
        };

        Fullscreen(common::Rect<std::int32_t, std::uint32_t> screen_a, const Properties properties_a)
            : Window()
        {
            this->created = this->create_window(screen_a, properties_a);
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
        bool create_window(common::Rect<std::int32_t, std::uint32_t> screen_a, const Properties& properties_a);
        void destroy();

        friend class lxf::Windower;
    };
};

constexpr canvas::Size operator|(canvas::Size type_a, common::Extent<std::uint16_t> size_a)
{
    assert(canvas::Size::custom == type_a);

    return static_cast<canvas::Size>(static_cast<std::uint16_t>(type_a) | (size_a.w << 16u) |
                                     (static_cast<std::uint64_t>(size_a.h) << 32u));
}
constexpr canvas::Size operator&(canvas::Size left_a, canvas::Size right_a)
{
    return static_cast<canvas::Size>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}

constexpr canvas::Position operator|(canvas::Position type_a, common::Position<std::uint16_t> position_a)
{
    assert(canvas::Position::custom == type_a);

    return static_cast<canvas::Position>(static_cast<std::uint16_t>(type_a) | (position_a.x << 16u) |
                                         (static_cast<std::uint64_t>(position_a.y) << 32u));
}
constexpr canvas::Position operator&(canvas::Position left_a, canvas::Position right_a)
{
    return static_cast<canvas::Position>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}
} // namespace lxf
