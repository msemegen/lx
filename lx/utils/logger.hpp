#pragma once

// lx
#include <lx/common/non_constructible.hpp>
#include <lx/containers/String.hpp>

// std
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <print>
#include <source_location>
#include <string_view>

namespace lx::utils {
class logger : private lx::common::non_constructible
{
public:
    enum class Kind : std::uint64_t
    {
        inf = 0x1u,
        wrn = 0x2u,
        err = 0x4u,
        omg = 0x8u,
        dbg = 0x10u,
    };

    using enum Kind;

private:
    struct Composer
    {
        void begin(Kind kind_a, const std::source_location& source_location_a);
        void end();

        template<typename... Args> void message(std::string_view format_a, Args... args_a)
        {
            if (static_cast<std::uint64_t>(this->kind) ==
                (static_cast<std::uint64_t>(logger::kind) & static_cast<std::uint64_t>(this->kind)))
            {
                const std::string message = std::vformat(std::locale {}, format_a.data(), std::make_format_args(args_a...));
                logger::log(message);
            }
        }

    private:
        Kind kind = Kind { 0x20u };
    };

public:
    static void set_filter(Kind kind_a)
    {
        kind = kind_a;
    }
    static Kind get_filter()
    {
        return kind;
    }
    static bool is_filter(Kind kind_a)
    {
        return static_cast<std::uint64_t>(kind_a) == (static_cast<std::uint64_t>(kind) & static_cast<std::uint64_t>(kind_a));
    }

    template<typename... Args>
    static void write_line(Kind kind_a, const std::source_location& source_location_a, std::string_view message_a, Args... args_a)
    {
        Composer c;

        c.begin(kind_a, source_location_a);
        c.message(message_a, args_a...);
        c.end();
    }

    inline static Composer composer;

private:
    static void log(std::string_view log_a);

    inline static Kind kind = Kind { 0x1Fu };
};

constexpr logger::Kind operator|(logger::Kind left_a, logger::Kind right_a)
{
    return static_cast<logger::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr logger::Kind operator&(logger::Kind left_a, logger::Kind right_a)
{
    return static_cast<logger::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
constexpr logger::Kind operator|=(logger::Kind& left_a, logger::Kind right_a)
{
    left_a = left_a | right_a;
    return left_a;
}
constexpr logger::Kind operator&=(logger::Kind& left_a, logger::Kind right_a)
{
    left_a = left_a & right_a;
    return left_a;
}
} // namespace lx::utils

#define log_set_filter(kind_a) lx::utils::logger::set_filter(kind_a);
#define log_inf(message_a, ...) \
    lx::utils::logger::write_line(lx::utils::logger::inf, std::source_location::current(), message_a, __VA_ARGS__)
#define log_wrn(message_a, ...) \
    lx::utils::logger::write_line(lx::utils::logger::wrn, std::source_location::current(), message_a, __VA_ARGS__)
#define log_err(message_a, ...) \
    lx::utils::logger::write_line(lx::utils::logger::err, std::source_location::current(), message_a, __VA_ARGS__)
#define log_omg(message_a, ...) \
    lx::utils::logger::write_line(lx::utils::logger::omg, std::source_location::current(), message_a, __VA_ARGS__)
#define log_dbg(message_a, ...) \
    lx::utils::logger::write_line(lx::utils::logger::dbg, std::source_location::current(), message_a, __VA_ARGS__)