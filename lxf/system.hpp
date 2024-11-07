#pragma once

/*
 *   Name: system.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <cstdint>
#include <string_view>

// platform
#include <Windows.h>

// lxf
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/various.hpp>

namespace lxf {
struct system : common::non_constructible
{
    struct message_box : common::non_constructible
    {
        enum class Icon : std::uint32_t
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

        enum class Button : std::uint32_t
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

        static Button show(std::string_view title, std::string_view message, Icon icon, Button buttons)
        {
            std::uint32_t ret = MessageBox(
                HWND_DESKTOP, message.data(), title.data(), static_cast<std::uint32_t>(icon) | get_win32_button_flag(buttons));

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
        static std::uint32_t get_win32_button_flag(Button buttons);
    };
};

constexpr system::message_box::Button operator|(system::message_box::Button left, system::message_box::Button right)
{
    return static_cast<system::message_box::Button>(static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
}
constexpr system::message_box::Button operator&(system::message_box::Button left, system::message_box::Button right)
{
    return static_cast<system::message_box::Button>(static_cast<std::uint32_t>(left) & static_cast<std::uint32_t>(right));
}

inline std::uint32_t system::message_box::get_win32_button_flag(Button buttons)
{
    switch (static_cast<std::uint32_t>(buttons))
    {
        case static_cast<std::uint32_t>(Button::ok):
            return MB_OK;

        case static_cast<std::uint32_t>(Button::help):
            return MB_HELP;

        case static_cast<std::uint32_t>(Button::ok | Button::cancel):
            return MB_OKCANCEL;

        case static_cast<std::uint32_t>(Button::yes | Button::no):
            return MB_YESNO;

        case static_cast<std::uint32_t>(Button::retry | Button::cancel):
            return MB_RETRYCANCEL;

        case static_cast<std::uint32_t>(Button::yes | Button::no | Button::cancel):
            return MB_YESNOCANCEL;

        case static_cast<std::uint32_t>(Button::abort | Button::retry | Button::ignore):
            return MB_ABORTRETRYIGNORE;

        case static_cast<std::uint32_t>(Button::cancel | Button::retry | Button::ignore):
            return MB_CANCELTRYCONTINUE;

        default: {
            assert(false);
            return 0x0ull;
        }
    }
    _assume(false);
}

} // namespace lxf