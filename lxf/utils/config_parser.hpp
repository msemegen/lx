#pragma once

/*
 *   Name: config_parser.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cstdint>
#include <string_view>
#include <vector>

// lx
#include <lxf/common/non_constructible.hpp>

namespace lxf {
namespace utils {
struct config_parser : private common::non_constructible
{
private:
    static constexpr std::string_view white_spces = " \r\n\t";
    static constexpr std::string_view separators = ":=[],";
    static constexpr char string_delimeter = '\"';

public:
    static void allocate(std::string_view data);
    static void free();

    static [[nodiscard]] std::vector<std::string_view> get_value(const std::vector<std::string_view>& name)
    {
        for (const Entry& entry : entries)
        {
            if (entry.name == name)
            {
                return entry.value;
            }
        }
        return {};
    }

private:
    struct Token
    {
        enum class Kind : std::uint64_t
        {
            name,
            separator,
            value
        };

        std::string_view value;
        Kind kind;
    };

    struct Entry
    {
        std::vector<std::string_view> name;
        std::vector<std::string_view> value;
    };

    static [[nodiscard]] Token::Kind get_token_kind(std::string_view token)
    {
        if (token.length() == 1u && true == contains(separators, token[0]))
        {
            return Token::Kind::separator;
        }

        if (token.length() > 1u && string_delimeter == token.front() && string_delimeter == token.back())
        {
            return Token::Kind::value;
        }

        return Token::Kind::name;
    }
    static [[nodiscard]] bool contains(std::string_view data, char c)
    {
        for (char character : data)
        {
            if (character == c)
            {
                return true;
            }
        }

        return false;
    }

    static [[nodiscard]] std::uint64_t calculate_tail(std::string_view data)
    {
        std::uint64_t ret = 0;
        bool in_string = false;

        for (std::uint64_t i = 0; i + 1ull < data.length(); i++)
        {
            if (string_delimeter == data[i])
            {
                in_string = !in_string;
            }

            if (false == in_string)
            {
                if (false == contains(white_spces, data[i]))
                {
                    if (true == contains(separators, data[i + 1ull]))
                    {
                        ret++;
                    }
                    else if ((true == contains(separators, data[i]) || string_delimeter == data[i]) &&
                             false == contains(white_spces, data[i + 1u]))
                    {
                        ret++;
                    }
                }
            }
        }

        return ret;
    }

    static [[nodiscard]] std::vector<Token> tokenize();
    static [[nodiscard]] std::vector<Entry> build(const std::vector<Token>& tokens);

    inline static char* p_storage = nullptr;
    inline static std::uint64_t storage_capacity = 0ull;

    inline static std::vector<Entry> entries;
};
} // namespace utils
} // namespace lxf