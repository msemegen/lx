// this
#include <lx/Windower.hpp>

extern VkInstance vk_instance;

namespace {
constexpr std::string_view lxf_window_class_name = "lxf_window_class";
} // namespace

namespace lx {
using namespace lx::common;

LRESULT __stdcall Windower::window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

Windower::Windower()
    : events {}
{
    WNDCLASSEX window_class_descriptor {};
    window_class_descriptor.cbSize = sizeof(WNDCLASSEXW);
    window_class_descriptor.style = CS_HREDRAW | CS_VREDRAW;
    window_class_descriptor.lpfnWndProc = Windower::window_procedure;
    window_class_descriptor.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class_descriptor.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class_descriptor.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    window_class_descriptor.lpszClassName = lxf_window_class_name.data();

    this->wnd_class = RegisterClassEx(&window_class_descriptor);
}

Windower::~Windower()
{
    UnregisterClass(MAKEINTATOM(this->wnd_class), GetModuleHandle(nullptr));
}

template<>
Canvas<Windower::Kind::framed>* Windower::create<Windower::Kind::framed>(const lx::devices::Display& display_a,
                                                                         const Canvas<Windower::Kind::framed>::Properties& properties_a)
{
    assert(properties_a.size.w > 0u && properties_a.size.h > 0u);

    const Position<std::uint64_t, 2u> position {
        .x = display_a.logical_rect.position.x + (static_cast<std::int64_t>(display_a.logical_rect.size.w) / 2u) - properties_a.size.w / 2u,
        .y = display_a.logical_rect.position.y + (static_cast<std::int64_t>(display_a.logical_rect.size.h) / 2u) - properties_a.size.h / 2u
    };

    HWND handle = CreateWindowEx(0u,
                                 MAKEINTATOM(this->wnd_class),
                                 properties_a.title.data(),
                                 WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION,
                                 static_cast<std::int32_t>(position.x),
                                 static_cast<std::int32_t>(position.y),
                                 static_cast<std::int32_t>(properties_a.size.w),
                                 static_cast<std::int32_t>(properties_a.size.h),
                                 HWND_DESKTOP,
                                 nullptr,
                                 GetModuleHandle(nullptr),
                                 nullptr);

    auto canvas = new Canvas<Windower::Kind::framed> { handle, properties_a };
    this->framed_windows[canvas] = handle;

    SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(canvas));

    return canvas;
}

template<> void Windower::destroy<Windower::Kind::framed>(out<Canvas<Windower::Kind::framed>*> canvas_a)
{
    (*canvas_a)->destroy();
    DestroyWindow(this->framed_windows[*canvas_a]);

    // TODO: remove canvas from framed_windows

    delete (*canvas_a);
    (*canvas_a) = nullptr;
}

Canvas<Windower::framed>::Canvas(HWND window_handle_a, const Properties& properties_a)
    : properties(properties_a)
{
    VkWin32SurfaceCreateInfoKHR vk_surface_create_info = { .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                                                           .pNext = nullptr,
                                                           .flags = 0,
                                                           .hinstance = GetModuleHandle(nullptr),
                                                           .hwnd = window_handle_a };

    vkCreateWin32SurfaceKHR(vk_instance, &vk_surface_create_info, nullptr, &(this->vk_surface));
}

void Canvas<Windower::framed>::destroy()
{
    vkDestroySurfaceKHR(vk_instance, this->vk_surface, nullptr);
    this->vk_surface = VK_NULL_HANDLE;

    ChangeDisplaySettings(nullptr, 0);
}

template<> bool Windower::update<Windower::Kind::framed>(HWND window_handle_a)
{
    MSG msg;
    auto canvas = reinterpret_cast<Canvas<Windower::Kind::framed>*>(GetWindowLongPtr(window_handle_a, GWLP_USERDATA));

    if (nullptr != canvas)
    {
        if (TRUE == PeekMessage(&msg, window_handle_a, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }

        return true;
    }
    return false;
}
template<> bool Windower::update<Windower::Kind::fullscreen>(HWND window_handle_a)
{
    MSG msg;
    auto canvas = reinterpret_cast<Canvas<Windower::Kind::fullscreen>*>(GetWindowLongPtr(window_handle_a, GWLP_USERDATA));

    if (nullptr != canvas)
    {
        if (TRUE == PeekMessage(&msg, window_handle_a, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }

        return true;
    }
    return false;
}

void Windower::set_visible(HWND windows_handle_a, bool visible)
{
    ShowWindow(windows_handle_a, true == visible ? SW_SHOW : SW_HIDE);
}
} // namespace lx