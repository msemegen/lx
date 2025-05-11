// this
#include <lx/Windower.hpp>

extern VkInstance vk_instance;

namespace {
constexpr std::string_view lx_framed_window_class_name = "lx_framed_window_class";
constexpr std::string_view lx_fullscreen_window_class_name = "lx_fullscreen_window_class";
} // namespace

namespace lx {
using namespace lx::common;

LRESULT __stdcall Windower::framed_window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
    auto canvas = reinterpret_cast<Canvas<Windower::Kind::framed>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
        case WM_CLOSE: {
            canvas->running = false;
        }
        break;
        case WM_SIZE: {
            if (nullptr != canvas->p_on_size_change)
            {
                canvas->p_on_size_change->on_size_change();
            }

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

        case WM_MOVE: {
            if (nullptr != canvas->p_on_position_change)
            {
                canvas->p_on_position_change->on_position_change();
            }
        }

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
LRESULT __stdcall Windower::fullscreen_window_procedure(HWND hwnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
    auto canvas = reinterpret_cast<Canvas<Windower::Kind::fullscreen>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
        case WM_QUIT: {
            canvas->running = false;
        }
        break;
        case WM_SIZE: {
            if (nullptr != canvas->p_on_size_change)
            {
                canvas->p_on_size_change->on_size_change();
            }

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

        case WM_MOVE: {
        }

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
    WNDCLASSEX framed_window_class_descriptor { .cbSize = sizeof(WNDCLASSEXW),
                                                .style = CS_HREDRAW | CS_VREDRAW,
                                                .lpfnWndProc = Windower::framed_window_procedure,
                                                .hIcon = LoadIcon(NULL, IDI_APPLICATION),
                                                .hCursor = LoadCursor(NULL, IDC_ARROW),
                                                .hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
                                                .lpszClassName = lx_framed_window_class_name.data() };
    WNDCLASSEX fullscreen_window_class_descriptor { .cbSize = sizeof(WNDCLASSEXW),
                                                    .style = CS_HREDRAW | CS_VREDRAW,
                                                    .lpfnWndProc = Windower::fullscreen_window_procedure,
                                                    .hIcon = LoadIcon(NULL, IDI_APPLICATION),
                                                    .hCursor = LoadCursor(NULL, IDC_ARROW),
                                                    .hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
                                                    .lpszClassName = lx_framed_window_class_name.data() };

    this->framed_wnd_class = RegisterClassEx(&framed_window_class_descriptor);
    this->fullscreen_wnd_class = RegisterClassEx(&framed_window_class_descriptor);
}

Windower::~Windower()
{
    UnregisterClass(MAKEINTATOM(this->framed_wnd_class), GetModuleHandle(nullptr));
    UnregisterClass(MAKEINTATOM(this->fullscreen_wnd_class), GetModuleHandle(nullptr));
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
                                 MAKEINTATOM(this->framed_wnd_class),
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
    this->running = true;
}

void Canvas<Windower::framed>::destroy()
{
    vkDestroySurfaceKHR(vk_instance, this->vk_surface, nullptr);
    this->vk_surface = VK_NULL_HANDLE;

    ChangeDisplaySettings(nullptr, 0);
}

bool Windower::update(const Canvas<Kind::framed>* canvas_a)
{
    MSG msg;

    if (false == canvas_a->running)
    {
        return false;
    }

    if (TRUE == PeekMessage(&msg, this->framed_windows[canvas_a], 0, 0, PM_REMOVE))
    {
        DispatchMessage(&msg);
    }

    return true;
}
bool Windower::update(const Canvas<Kind::fullscreen>* canvas_a)
{
    MSG msg;

    if (false == canvas_a->running)
    {
        return false;
    }

    if (TRUE == PeekMessage(&msg, this->fullscreen_windows[canvas_a], 0, 0, PM_REMOVE))
    {
        DispatchMessage(&msg);
    }

    return true;
}

void Windower::set_visible(HWND windows_handle_a, bool visible)
{
    ShowWindow(windows_handle_a, true == visible ? SW_SHOW : SW_HIDE);
}

void Windower::Events::SizeChange::register_callback(inout<Canvas<Kind::framed>*> canvas, Callback* p_callback_a)
{
    (*canvas)->p_on_size_change = p_callback_a;
}
void Windower::Events::PositionChange::register_callback(inout<Canvas<Kind::framed>*> canvas, Callback* p_callback_a)
{
    (*canvas)->p_on_position_change = p_callback_a;
}
} // namespace lx