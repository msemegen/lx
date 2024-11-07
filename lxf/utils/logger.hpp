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

    template<typename... Args_t> static void write(Level level, std::string_view module_name, std::string_view format, Args_t... args)
    {
        if ((true == common::bit::flag::is(static_cast<std::uint64_t>(severity), static_cast<std::uint8_t>(level)) ||
             true == common::bit::flag::is(static_cast<std::uint64_t>(severity), 0x10ull)) &&
            true == is_open())
        {
            timestamp(std::time(nullptr));
            log(level, module_name, std::vformat(std::locale {}, format, std::make_format_args(args...)));
        }
    }
    template<typename... Args> static void write_line(Level level, std::string_view module_name, std::string_view format, Args... args)
    {
        if ((true == common::bit::flag::is(static_cast<std::uint64_t>(severity), static_cast<std::uint8_t>(level))) && true == is_open())
        {
            timestamp(std::time(nullptr));
            log(level, module_name, std::vformat(std::locale {}, format.data(), std::make_format_args(args...)));
            nl();
        }
    }

    static void set_severity(Severity sev)
    {
        common::bit::flag::set(reinterpret_cast<std::uint64_t*>(&logger::severity), 0x1Full, static_cast<std::uint64_t>(sev));
    }
    static void clear_severity(Severity sev)
    {
        common::bit::flag::clear(reinterpret_cast<std::uint64_t*>(&logger::severity), static_cast<std::uint64_t>(sev));
    }

    static void set_console_output(bool flag)
    {
        redirect_to_console = flag;
    }

    static bool is_open();
    static bool is_console_output()
    {
        return redirect_to_console;
    }

private:
    static const char* to_string(Level level);

    static void timestamp(std::time_t timestamp);
    static void log(Level level, std::string_view module_name, const std::string& log);
    static void nl();

    inline static Severity severity = Severity::none;
    inline static bool redirect_to_console = false;
};

constexpr logger::Severity operator|(logger::Severity left, logger::Severity right)
{
    return static_cast<logger::Severity>(static_cast<std::uint64_t>(left) | static_cast<std::uint64_t>(right));
}
constexpr logger::Severity operator&(logger::Severity left, logger::Severity right)
{
    return static_cast<logger::Severity>(static_cast<std::uint64_t>(left) & static_cast<std::uint64_t>(right));
}
} // namespace utils
} // namespace lxf