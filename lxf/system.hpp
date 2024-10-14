#pragma once

/*
 *   Name: system.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <string_view>

// platform
#include <Windows.h>

// lxf
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/scalar.hpp>
#include <lxf/common/various.hpp>

namespace lxf {
struct system : common::non_constructible
{
    struct message_box : common::non_constructible
    {
        enum class Icon : common::Uint32
        {
            exclamation = MB_ICONEXCLAMATION,
            warning = MB_ICONWARNING,
            info = MB_ICONINFORMATION,
            asterisk = MB_ICONASTERISK,
            question = MB_ICONQUESTION,
            stop = MB_ICONSTOP,
            error = MB_ICONERROR,
            hand = MB_ICONHAND
        };

        enum class Button : common::Uint32
        {
            ok = 0x1u,
            cancel = 0x2u,
            abort = 0x4u,
            retry = 0x8u,
            ignore = 0x10u,
            yes = 0x20,
            no = 0x40,
            help = 0x80
        };

        static Button show(std::string_view title_a, std::string_view message_a, Icon icon_a, Button buttons_a)
        {
            common::Uint32 ret = MessageBox(
                HWND_DESKTOP, message_a.data(), title_a.data(), static_cast<common::Uint32>(icon_a) | get_win32_button_flag(buttons_a));

            switch (ret)
            {
                case IDOK:
                    return Button::ok;

                case IDCANCEL:
                    return Button::cancel;

                case IDABORT:
                    return Button::abort;

                case IDRETRY:
                    return Button::retry;

                case IDIGNORE:
                    return Button::ignore;

                case IDYES:
                    return Button::yes;

                case IDNO:
                    return Button::no;

                case IDHELP:
                    return Button::help;
            }

            return common::various::get_enum_incorrect_value<Button>();
        }

    private:
        static common::Uint32 get_win32_button_flag(Button buttons_a);
    };
};

constexpr system::message_box::Button operator|(system::message_box::Button left_a, system::message_box::Button right_a)
{
    return static_cast<system::message_box::Button>(static_cast<common::Uint32>(left_a) | static_cast<common::Uint32>(right_a));
}
constexpr system::message_box::Button operator&(system::message_box::Button left_a, system::message_box::Button right_a)
{
    return static_cast<system::message_box::Button>(static_cast<common::Uint32>(left_a) & static_cast<common::Uint32>(right_a));
}

inline common::Uint32 system::message_box::get_win32_button_flag(Button buttons_a)
{
    switch (static_cast<common::Uint32>(buttons_a))
    {
        case static_cast<common::Uint32>(Button::ok):
            return MB_OK;

        case static_cast<common::Uint32>(Button::help):
            return MB_HELP;

        case static_cast<common::Uint32>(Button::ok | Button::cancel):
            return MB_OKCANCEL;

        case static_cast<common::Uint32>(Button::yes | Button::no):
            return MB_YESNO;

        case static_cast<common::Uint32>(Button::retry | Button::cancel):
            return MB_RETRYCANCEL;

        case static_cast<common::Uint32>(Button::yes | Button::no | Button::cancel):
            return MB_YESNOCANCEL;

        case static_cast<common::Uint32>(Button::abort | Button::retry | Button::ignore):
            return MB_ABORTRETRYIGNORE;

        case static_cast<common::Uint32>(Button::cancel | Button::retry | Button::ignore):
            return MB_CANCELTRYCONTINUE;

        default: {
            assert(false);
            return 0x0ull;
        }
    }
    __assume(false);
}

} // namespace lxf