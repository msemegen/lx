/*
 *   Name: config_parser.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/utils/config_parser.hpp>

// lfx
#include <lxf/common/various.hpp>

namespace lxf {
namespace utils {
using namespace ::lxf::common;

void config_parser::allocate(std::string_view data_a)
{
    free();

    storage_capacity = data_a.length() + calculate_tail(data_a) + 1ull;
    p_storage = new char[storage_capacity];
    std::memcpy(p_storage, data_a.data(), data_a.length());
    p_storage[data_a.length()] = '\0';

    std::vector<Token> tokens = tokenize();
    entries = build(tokens);
}

void config_parser::free()
{
    delete[] p_storage;
    p_storage = nullptr;
    storage_capacity = 0ull;
}

[[nodiscard]] std::vector<config_parser::Token> config_parser::tokenize()
{
    std::vector<Token> ret;

    std::string_view data = p_storage;

    std::uint64_t start = 0ull;
    for (; start < data.length() && true == contains(white_spces, data[start]); start++) continue;

    bool push_new = true;
    std::uint64_t i = start;

    // splt storage into '\0' seperated parts
    for (; i + 1ull < storage_capacity; i++)
    {
        if (string_delimeter == p_storage[i])
        {
            push_new = !push_new;
        }

        if (true == push_new)
        {
            if (true == contains(white_spces, p_storage[i]))
            {
                if ('\0' != p_storage[i - 1ull])
                {
                    p_storage[i] = '\0';
                }
                else
                {
                    std::memmove(p_storage + i, p_storage + i + 1ull, storage_capacity - i);
                    i--;
                }
            }
            else
            {
                if (true == contains(separators, p_storage[i + 1ull]))
                {
                    std::memmove(p_storage + i + 1ull, p_storage + i, storage_capacity - i);
                    p_storage[++i] = '\0';
                }
                else if ((true == contains(separators, p_storage[i]) || string_delimeter == p_storage[i]) &&
                         false == contains(white_spces, p_storage[i + 1u]))
                {
                    std::memmove(p_storage + i + 1ull, p_storage + i, storage_capacity - i);
                    p_storage[++i] = '\0';
                }
            }
        }
    }

    // make
    i = start;
    for (; i + 1ull < storage_capacity; i++)
    {
        std::uint64_t m = 0ull;
        if ('\0' == p_storage[i])
        {
            m = 0ull;
            Token::Kind kind = get_token_kind(std::string_view { p_storage + start, p_storage + i });

            // removing string delimeters from value type token
            if (Token::Kind::value == kind)
            {
                p_storage[start] = '\0';
                p_storage[i - 1ull] = '\0';
                start += 1ull;
                m = 1ull;
            }

            const Token t { .value { p_storage + start, p_storage + i - m }, .kind = kind };
            ret.push_back(t);
            start = i + 1ull;
        }
    }

    if (start + 1u < storage_capacity)
    {
        Token::Kind kind = get_token_kind(std::string_view { p_storage + start, p_storage + i });

        if (Token::Kind::name == kind || Token::Kind::separator == kind)
        {
            ret.push_back({ .value { p_storage + start, p_storage + storage_capacity - 1ull }, .kind = kind });
        }
        else
        {
            // removing string delimeters from value type token
            p_storage[start] = '\0';
            p_storage[storage_capacity - 2ull] = '\0';
            ret.push_back({ .value { p_storage + start + 1ull, p_storage + storage_capacity - 2ull }, .kind = kind });
        }
    }

    return ret;
}

[[nodiscard]] std::vector<config_parser::Entry> config_parser::build(const std::vector<Token>& tokens_a)
{
    if (true == tokens_a.empty())
    {
        return {};
    }

    std::vector<Entry> ret;

    Entry entry;
    Token::Kind last = various::get_enum_incorrect_value<Token::Kind>();

    for (std::uint64_t i = 0; i < tokens_a.size(); i++)
    {
        Token::Kind current = tokens_a[i].kind;

        // simple syntax checking
        if (Token::Kind::name == current &&
            (last == various::get_enum_incorrect_value<Token::Kind>() ||
             (Token::Kind::separator == last && ":" == tokens_a[i - 1ull].value) || Token::Kind::value == last))
        {
            if (false == entry.value.empty())
            {
                ret.push_back(entry);
                entry = {};
            }
            entry.name.push_back(tokens_a[i].value);
        }
        else if (false == entry.name.empty() &&
                 (last == Token::Kind::separator && ("=" == tokens_a[i - 1ull].value || "," == tokens_a[i - 1ull].value)) &&
                 Token::Kind::value == current)
        {
            entry.value.push_back(tokens_a[i].value);
        }

        last = current;
    }

    if (false == entry.name.empty())
    {
        ret.push_back(entry);
    }

    return ret;
}
} // namespace utils
} // namespace lxf