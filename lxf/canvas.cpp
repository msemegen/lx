/*
 *   Name: Canvas.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/Canvas.hpp>

// std
#include <string_view>

// lxf
#include <lxf/loader.hpp>

extern VkInstance vk_instance;

namespace {
constexpr std::string_view lxf_window_class_name = "lxf_window_class";

LRESULT __stdcall window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZE: {
            switch (wParam)
            {
                case SIZE_RESTORED: {
                }
                break;

                case SIZE_MINIMIZED: {
                }
                break;
            }
            return 0;
        }
        break;

        case WM_KILLFOCUS: {
        }
        break;

        case WM_SETFOCUS: {
        }
        break;

        case WM_DISPLAYCHANGE: {
            // TODO: detect screen addition / removal
        }
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

} // namespace

namespace lxf {
using namespace common;

bool canvas::Windowed::create_window(const device::Display* p_display_a, const Descriptor& descriptor_a)
{
    WNDCLASSEX window_class_descriptor {};
    window_class_descriptor.cbSize = sizeof(WNDCLASSEXW);
    window_class_descriptor.style = CS_HREDRAW | CS_VREDRAW;
    window_class_descriptor.lpfnWndProc = window_procedure;
    window_class_descriptor.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class_descriptor.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class_descriptor.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    window_class_descriptor.lpszClassName = lxf_window_class_name.data();

    this->wnd_class = RegisterClassEx(&window_class_descriptor);

    common::Size<std::uint16_t> wnd_size;
    common::Position<std::uint16_t> wnd_pos;

    assert(Position::automatic != (descriptor_a.position & Position::automatic) || Size::custom != (descriptor_a.size & Size::custom));

    if (Position::custom == (descriptor_a.position & Position::custom) && Size::custom == (descriptor_a.size & Size::custom))
    {
        const std::uint32_t size_tmp = static_cast<std::uint32_t>(static_cast<std::uint64_t>(descriptor_a.size) >> 16u);
        std::memcpy(&wnd_size, &size_tmp, sizeof(wnd_size));

        const std::uint32_t position_tmp = static_cast<std::uint32_t>(static_cast<std::uint64_t>(descriptor_a.position) >> 16u);
        std::memcpy(&wnd_pos, &position_tmp, sizeof(wnd_pos));
    }
    else if (Position::centered == (descriptor_a.position & Position::centered) && Size::custom == (descriptor_a.size & Size::custom))
    {
        device::Display::Properties display_properties = p_display_a->get_properties();
        const std::uint32_t size_tmp = static_cast<std::uint32_t>(static_cast<std::uint64_t>(descriptor_a.size) >> 16u);
        std::memcpy(&wnd_size, &size_tmp, sizeof(wnd_size));

        wnd_pos = { .x = static_cast<std::uint16_t>(display_properties.logical_rect.size.w / 2u - wnd_size.w / 2u),
                    .y = static_cast<std::uint16_t>(display_properties.logical_rect.size.h / 2u - wnd_size.h / 2u) };
    }
    else if (Position::automatic == (descriptor_a.position & Position::automatic) && Size::maximize == (descriptor_a.size & Size::maximize))
    {
        wnd_pos = { .x = 0u, .y = 0u };
    }

    this->handle = CreateWindowEx(0u,
                                  MAKEINTATOM(this->wnd_class),
                                  descriptor_a.title.data(),
                                  WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION,
                                  wnd_pos.x,
                                  wnd_pos.y,
                                  wnd_size.w,
                                  wnd_size.h,
                                  HWND_DESKTOP,
                                  nullptr,
                                  GetModuleHandle(nullptr),
                                  nullptr);

    if (nullptr != this->handle)
    {
        VkWin32SurfaceCreateInfoKHR vk_surface_create_info = { .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                                                               .pNext = nullptr,
                                                               .flags = 0,
                                                               .hinstance = GetModuleHandle(nullptr),
                                                               .hwnd = this->handle };

        return VK_SUCCESS == vkCreateWin32SurfaceKHR(vk_instance, &vk_surface_create_info, nullptr, &(this->vk_surface));
    }

    return false;
}
void canvas::Windowed::set_visible(bool visible_a)
{
    ShowWindow(this->handle, true == visible_a ? SW_SHOW : SW_HIDE);
}

bool canvas::Windowed::update()
{
    MSG msg;

    if (TRUE == PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
        {
            return false;
        }
    }

    return true;
}

void canvas::Windowed::destroy()
{
    vkDestroySurfaceKHR(vk_instance, this->vk_surface, nullptr);

    DestroyWindow(this->handle);
    UnregisterClass(MAKEINTATOM(this->wnd_class), GetModuleHandle(nullptr));
}

void canvas::Fullscreen::destroy()
{
    vkDestroySurfaceKHR(vk_instance, this->vk_surface, nullptr);

    DestroyWindow(this->handle);
    UnregisterClass(MAKEINTATOM(this->wnd_class), GetModuleHandle(nullptr));
}
} // namespace lxf