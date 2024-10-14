#pragma once

/*
 *   Name: config_parser.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <string_view>
#include <vector>

// lx
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/scalar.hpp>

namespace lxf {
namespace utils {
struct config_parser : private common::non_constructible
{
private:
    static constexpr std::string_view white_spces = " \r\n\t";
    static constexpr std::string_view separators = ":=[],";
    static constexpr char string_delimeter = '\"';

public:
    static void allocate(std::string_view data_a);
    static void free();

    static [[nodiscard]] std::vector<std::string_view> get_value(const std::vector<std::string_view>& name_a)
    {
        for (const Entry& entry : entries)
        {
            if (entry.name == name_a)
            {
                return entry.value;
            }
        }
        return {};
    }

private:
    struct Token
    {
        enum class Kind : common::Uint64
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

    static [[nodiscard]] Token::Kind get_token_kind(std::string_view token_a)
    {
        if (token_a.length() == 1u && true == contains(separators, token_a[0]))
        {
            return Token::Kind::separator;
        }

        if (token_a.length() > 1u && string_delimeter == token_a.front() && string_delimeter == token_a.back())
        {
            return Token::Kind::value;
        }

        return Token::Kind::name;
    }
    static [[nodiscard]] bool contains(std::string_view data_a, char c_a)
    {
        for (wchar_t c : data_a)
        {
            if (c == c_a)
            {
                return true;
            }
        }

        return false;
    }

    static [[nodiscard]] common::Uint64 calculate_tail(std::string_view data_a)
    {
        common::Uint64 ret = 0;
        bool in_string = false;

        for (common::Uint64 i = 0; i + 1ull < data_a.length(); i++)
        {
            if (string_delimeter == data_a[i])
            {
                in_string = !in_string;
            }

            if (false == in_string)
            {
                if (false == contains(white_spces, data_a[i]))
                {
                    if (true == contains(separators, data_a[i + 1ull]))
                    {
                        ret++;
                    }
                    else if ((true == contains(separators, data_a[i]) || string_delimeter == data_a[i]) &&
                             false == contains(white_spces, data_a[i + 1u]))
                    {
                        ret++;
                    }
                }
            }
        }

        return ret;
    }

    static [[nodiscard]] std::vector<Token> tokenize();
    static [[nodiscard]] std::vector<Entry> build(const std::vector<Token>& tokens_a);


    inline static char* p_storage = nullptr;
    inline static common::Uint64 storage_capacity = 0ull;

    inline static std::vector<Entry> entries;
};
} // namespace utils
} // namespace lxf