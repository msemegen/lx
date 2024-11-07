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

        Windowed(common::Rect<std::int32_t, std::uint32_t> screen, const Properties& properties)
            : Window()
        {
            this->created = this->create_window(screen, properties);
        }

        void set_visible(bool visible);
        bool update();

        bool operator==(const Windowed& obj)
        {
            return this->handle == obj.handle && this->wnd_class == obj.wnd_class;
        }
        bool operator!=(const Windowed& obj)
        {
            return false == (*this == obj);
        }

    private:
        bool create_window(common::Rect<std::int32_t, std::uint32_t> screen, const Properties& properties);
        void destroy();

        friend class lxf::Windower;
    };
    class Fullscreen : public Window
    {
    public:
        struct Properties
        {
            common::Extent<std::uint32_t, 2u> resolution;
            std::uint8_t bits_per_pixel;
        };

        Fullscreen(common::Rect<std::int32_t, std::uint32_t> screen, const Properties properties)
            : Window()
        {
            this->created = this->create_window(screen, properties);
        }

        void set_visible(bool visible);
        bool update();

        bool operator==(const Fullscreen& obj)
        {
            return this->handle == obj.handle && this->wnd_class == obj.wnd_class;
        }
        bool operator!=(const Fullscreen& obj)
        {
            return false == (*this == obj);
        }

    private:
        bool create_window(common::Rect<std::int32_t, std::uint32_t> screen, const Properties& properties);
        void destroy();

        friend class lxf::Windower;
    };
};

constexpr canvas::Size operator|(canvas::Size type, common::Extent<std::uint16_t, 2u> size)
{
    assert(canvas::Size::custom == type);

    return static_cast<canvas::Size>(static_cast<std::uint16_t>(type) | (size.w << 16u) |
                                     (static_cast<std::uint64_t>(size.h) << 32u));
}
constexpr canvas::Size operator&(canvas::Size left, canvas::Size right)
{
    return static_cast<canvas::Size>(static_cast<std::uint32_t>(left) & static_cast<std::uint32_t>(right));
}

constexpr canvas::Position operator|(canvas::Position type, common::Position<std::uint16_t, 2u> position)
{
    assert(canvas::Position::custom == type);

    return static_cast<canvas::Position>(static_cast<std::uint16_t>(type) | (position.x << 16u) |
                                         (static_cast<std::uint64_t>(position.y) << 32u));
}
constexpr canvas::Position operator&(canvas::Position left, canvas::Position right)
{
    return static_cast<canvas::Position>(static_cast<std::uint32_t>(left) & static_cast<std::uint32_t>(right));
}
} // namespace lxf
