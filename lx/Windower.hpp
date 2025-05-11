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
template<auto Kind> class Canvas : private lx::common::non_constructible
{
};

class Windower : private lx::common::non_copyable
{
public:
    enum class Kind
    {
        framed,
        fullscreen
    };

    struct Events
    {
        struct SizeChange
        {
            void register_callback() {}
        } size_size;
        struct PositionChange
        {
            void register_callback() {}
        } position_change;
    } events;

    using enum Kind;

    Windower();
    ~Windower();

    bool update(const Canvas<Kind::framed>* canvas_a);
    bool update(const Canvas<Kind::fullscreen>* canvas_a);

    void set_visible(const Canvas<Kind::framed>* canvas_a, bool visible_a);
    void set_visible(const Canvas<Kind::fullscreen>* canvas_a, bool visible_a);

    template<auto Kind>
    Canvas<Kind>* create(const lx::devices::Display& display_a, typename const Canvas<Kind>::Properties& properties_a) = delete;
    template<auto Kind> void destroy(lx::common::out<Canvas<Kind>*> canvas_a) = delete;

private:
    template<auto Kind> bool update(HWND window_handle) = delete;
    void set_visible(HWND window_handle_a, bool visible_a);

    static LRESULT __stdcall window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam);

    ATOM wnd_class = 0u;

    std::unordered_map<const Canvas<Kind::framed>*, HWND> framed_windows;
    std::unordered_map<const Canvas<Kind::fullscreen>*, HWND> fullscreen_windows;

    friend lx::gpu::Device;
};

template<> class Canvas<Windower::framed>
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

    friend Windower;
    friend lx::gpu::Device;
};
template<> class Canvas<Windower::fullscreen>
{
public:
    struct Properties
    {
        common::Extent<std::uint64_t, 2u> size;
        std::uint8_t bbp = 0u;
    };

private:
    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;

    HWND handle = nullptr;
    ATOM wnd_class = 0u;

    bool created = false;
    std::size_t id = std::numeric_limits<std::size_t>::max();

    friend Windower;
};

template<> bool Windower::update<Windower::Kind::framed>(HWND window_handle);
template<> bool Windower::update<Windower::Kind::fullscreen>(HWND window_handle);

inline bool Windower::update(const Canvas<Kind::framed>* canvas_a)
{
    return this->update<Kind::framed>(this->framed_windows[canvas_a]);
}
inline bool Windower::update(const Canvas<Kind::fullscreen>* canvas_a)
{
    return this->update<Kind::fullscreen>(this->fullscreen_windows[canvas_a]);
}

inline void Windower::set_visible(const Canvas<Kind::framed>* canvas_a, bool visible_a)
{
    this->set_visible(this->framed_windows[canvas_a], visible_a);
}
inline void Windower::set_visible(const Canvas<Kind::fullscreen>* canvas_a, bool visible_a)
{
    this->set_visible(this->fullscreen_windows[canvas_a], visible_a);
}

template<>
Canvas<Windower::Kind::framed>* Windower::create<Windower::Kind::framed>(const lx::devices::Display& display_a,
                                                                         const Canvas<Windower::Kind::framed>::Properties& properties_a);
template<> void Windower::destroy<Windower::Kind::framed>(lx::common::out<Canvas<Windower::Kind::framed>*> canvas_a);

template<> Canvas<Windower::Kind::fullscreen>*
Windower::create<Windower::Kind::fullscreen>(const lx::devices::Display& display_a,
                                             const Canvas<Windower::Kind::fullscreen>::Properties& properties_a);
} // namespace lx