#pragma once

// lx
#include <lx/common/inout.hpp>
#include <lx/common/non_constructible.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/common/out.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/Display.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cassert>
#include <unordered_map>
#include <utility>

namespace lx::gpu {
class Device;
}

namespace lx {
struct canvas : private lx::common::non_constructible
{
    enum class Kind
    {
        framed,
        fullscreen
    };

    using enum Kind;
};
template<auto Kind> class Canvas : private lx::common::non_constructible
{
};

class Windower : private lx::common::non_copyable
{
public:
    struct Events
    {
        struct SizeChange
        {
            class Callback
            {
            public:
                virtual bool on_size_change() = 0;
            };

            void register_callback(lx::common::inout<Canvas<canvas::framed>*> canvas, Callback* p_callback_a);
        } size_change;
        struct PositionChange
        {
            class Callback
            {
            public:
                virtual bool on_position_change(std::uint64_t pos_x_a, std::uint64_t pos_y_a) = 0;
            };

            void register_callback(lx::common::inout<Canvas<canvas::framed>*> canvas, Callback* p_callback_a);
        } position_change;
    } events;

    Windower();
    ~Windower();

    bool update(const Canvas<canvas::framed>* canvas_a);
    bool update(const Canvas<canvas::fullscreen>* canvas_a);

    void set_visible(const Canvas<canvas::framed>* canvas_a, bool visible_a);
    void set_visible(const Canvas<canvas::fullscreen>* canvas_a, bool visible_a);

    template<auto Kind>
    Canvas<Kind>* create(const lx::devices::Display& display_a, typename const Canvas<Kind>::Properties& properties_a) = delete;
    template<auto Kind> void destroy(lx::common::out<Canvas<Kind>*> canvas_a) = delete;

private:
    void set_visible(HWND window_handle_a, bool visible_a);

    static LRESULT __stdcall framed_window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);
    static LRESULT __stdcall fullscreen_window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);

    ATOM framed_wnd_class = 0u;
    ATOM fullscreen_wnd_class = 0u;

    std::unordered_map<const Canvas<canvas::framed>*, HWND> framed_windows;
    std::unordered_map<const Canvas<canvas::fullscreen>*, HWND> fullscreen_windows;

    friend lx::gpu::Device;
};

template<> class Canvas<canvas::framed>
{
public:
    struct Properties
    {
        std::string_view title;
        common::Extent<std::uint64_t, 2u> size;
    };

    bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_surface;
    }

    const Properties& get_properties() const
    {
        return this->properties;
    }

    operator VkSurfaceKHR() const
    {
        return this->vk_surface;
    }
    operator VkSurfaceKHR()
    {
        return this->vk_surface;
    }

private:
    Canvas() = default;
    Canvas(HWND window_handle_a, const Properties& properties_a);

    void destroy();

    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
    Properties properties;

    Windower::Events::SizeChange::Callback* p_on_size_change = nullptr;
    Windower::Events::PositionChange::Callback* p_on_position_change = nullptr;

    bool running = false;

    friend Windower;
    friend lx::gpu::Device;
};
template<> class Canvas<canvas::fullscreen>
{
public:
    struct Properties
    {
        common::Extent<std::uint64_t, 2u> size;
        std::uint8_t bbp = 0u;
    };

private:
    Canvas() = default;
    Canvas(HWND window_handle_a, const Properties& properties_a);

    void destroy();

    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
    Properties properties;

    Windower::Events::SizeChange::Callback* p_on_size_change = nullptr;
    Windower::Events::PositionChange::Callback* p_on_position_change = nullptr;

    bool running = false;

    friend Windower;
};

inline void Windower::set_visible(const Canvas<canvas::framed>* canvas_a, bool visible_a)
{
    this->set_visible(this->framed_windows[canvas_a], visible_a);
}
inline void Windower::set_visible(const Canvas<canvas::fullscreen>* canvas_a, bool visible_a)
{
    this->set_visible(this->fullscreen_windows[canvas_a], visible_a);
}

template<> Canvas<canvas::framed>* Windower::create<canvas::framed>(const lx::devices::Display& display_a,
                                                                    const Canvas<canvas::framed>::Properties& properties_a);
template<> void Windower::destroy<canvas::framed>(lx::common::out<Canvas<canvas::framed>*> canvas_a);

template<> Canvas<canvas::fullscreen>* Windower::create<canvas::fullscreen>(const lx::devices::Display& display_a,
                                                                            const Canvas<canvas::fullscreen>::Properties& properties_a);
} // namespace lx