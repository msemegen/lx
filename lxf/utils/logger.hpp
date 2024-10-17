#pragma once

/*
 *   Name: logger.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <format>
#include <string>
#include <string_view>

// lxf
#include <lxf/common/bit.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/various.hpp>

namespace lxf {
namespace utils {
class logger : private common::non_constructible
{
public:
    enum class Severity : std::uint64_t
    {
        none = 0x0u,
        info = 0x1u,
        warning = 0x2u,
        error = 0x4u,
        omg = 0x8u,
        debug = 0x10u
    };
    enum class Level : std::uint64_t
    {
        info = 0x1u,
        warning = 0x2u,
        error = 0x4u,
        omg = 0x8u,
        debug = 0x10u
    };

    using enum Level;

    template<typename... Args_t>
    static void write(Level level_a, std::string_view module_name_a, std::string_view format_a, Args_t... args_a)
    {
        if ((true == common::bit::flag::is(static_cast<std::uint64_t>(severity), static_cast<std::uint8_t>(level_a)) ||
             true == common::bit::flag::is(static_cast<std::uint64_t>(severity), 0x10ull)) &&
            true == is_open())
        {
            timestamp(std::time(nullptr));
            log(level_a, module_name_a, std::vformat(std::locale {}, format_a, std::make_format_args(args_a...)));
        }
    }
    template<typename... Args>
    static void write_line(Level level_a, std::string_view module_name_a, std::string_view format_a, Args... args_a)
    {
        if ((true == common::bit::flag::is(static_cast<std::uint64_t>(severity), static_cast<std::uint8_t>(level_a))) && true == is_open())
        {
            timestamp(std::time(nullptr));
            log(level_a, module_name_a, std::vformat(std::locale {}, format_a.data(), std::make_format_args(args_a...)));
            nl();
        }
    }

    static void set_severity(Severity severity_a)
    {
        common::bit::flag::set(reinterpret_cast<std::uint64_t*>(&severity), 0x1Full, static_cast<std::uint64_t>(severity_a));
    }
    static void clear_severity(Severity severity_a)
    {
        common::bit::flag::clear(reinterpret_cast<std::uint64_t*>(&severity), static_cast<std::uint64_t>(severity_a));
    }

    static void set_console_output(bool flag_a)
    {
        redirect_to_console = flag_a;
    }

    static bool is_open();
    static bool is_console_output()
    {
        return redirect_to_console;
    }

private:
    static const char* to_string(Level level_a);

    static void timestamp(std::time_t timestamp_a);
    static void log(Level level_a, std::string_view module_name_a, const std::string& log_a);
    static void nl();

    inline static Severity severity = Severity::none;
    inline static bool redirect_to_console = false;
};

constexpr logger::Severity operator|(logger::Severity left_a, logger::Severity right_a)
{
    return static_cast<logger::Severity>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr logger::Severity operator&(logger::Severity left_a, logger::Severity right_a)
{
    return static_cast<logger::Severity>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
} // namespace utils
} // namespace lxf