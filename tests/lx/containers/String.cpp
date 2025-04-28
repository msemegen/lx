// external
#include <catch2/catch_test_macros.hpp>

// lx
#include <lx/containers/String.hpp>

// std
#include <cstring>

TEST_CASE("String<char>: constructors", "[lx][containers][String<char>]")
{
    using namespace lx::containers;

    SECTION("Default constructor creates an empty string")
    {
        String<char> string;

        REQUIRE(0u == string.get_length());
        REQUIRE(1u == string.get_capacity());
        REQUIRE(0 == std::strncmp("", string.get_cstring(), 1u));
    }

    SECTION("Constructor from const char* initializes correctly")
    {
        String<char> string("test");

        REQUIRE(4u == string.get_length());
        REQUIRE(6u == string.get_capacity());
        REQUIRE(0 == std::strncmp("test", string.get_cstring(), string.get_length()));
    }
}
TEST_CASE("String<char>: push_back operations", "[lx][containers][String<char>]")
{
    using namespace lx::containers;

    SECTION("push_back(char) on empty string adds one character")
    {
        String<char> string;

        string.push_back('X');

        REQUIRE(1u == string.get_length());
        REQUIRE(2u == string.get_capacity());
        REQUIRE(0 == std::strncmp("X", string.get_cstring(), string.get_length()));
    }

    SECTION("push_back(char) multiple times on empty string")
    {
        String<char> string;

        string.push_back('X');
        string.push_back('Y');
        string.push_back('Z');
        string.push_back('0');
        string.push_back('1');
        string.push_back('2');

        REQUIRE(6u == string.get_length());
        REQUIRE(7u == string.get_capacity());
        REQUIRE(0 == std::strncmp("XYZ012", string.get_cstring(), string.get_length()));
    }

    SECTION("push_back(char) on pre-filled string appends character")
    {
        String<char> string("test");

        string.push_back('X');

        REQUIRE(5u == string.get_length());
        REQUIRE(6u == string.get_capacity());
        REQUIRE(0 == std::strncmp("testX", string.get_cstring(), string.get_length()));
    }

    SECTION("push_back(char) multiple times on pre-filled string")
    {
        String<char> string("test");

        string.push_back('X');
        string.push_back('Y');
        string.push_back('Z');
        string.push_back('0');
        string.push_back('1');
        string.push_back('2');

        REQUIRE(10u == string.get_length());
        REQUIRE(11u == string.get_capacity());
        REQUIRE(0 == std::strncmp("testXYZ012", string.get_cstring(), string.get_length()));
    }

    SECTION("push_back(const char*) on empty string adds entire literal")
    {
        String<char> string;

        string.push_back("test");

        REQUIRE(4u == string.get_length());
        REQUIRE(5u == string.get_capacity());
        REQUIRE(0 == std::strncmp("test", string.get_cstring(), string.get_length()));
    }

    SECTION("push_back(const char*) on pre-filled string appends literal")
    {
        String<char> string("test");

        string.push_back("XYZ012");

        REQUIRE(10u == string.get_length());
        REQUIRE(11u == string.get_capacity());
        REQUIRE(0 == std::strncmp("testXYZ012", string.get_cstring(), string.get_length()));
    }
}
TEST_CASE("String<char>: conversion to string_view", "[lx][containers][String<char>]")
{
    using namespace lx::containers;

    String<char> test("test");

    std::string_view view = test;

    REQUIRE(4u == view.size());
    REQUIRE("test" == view);
}

TEST_CASE("String<char, N>: construction", "[lx][containers][String<char, N>]")
{
    using namespace lx::containers;

    SECTION("Default constructor creates an empty static string")
    {
        String<char, 5u> string;

        REQUIRE(0u == string.get_length());
        REQUIRE(5u == string.get_capacity());
        REQUIRE(0 == std::strncmp("", string.get_cstring(), 1u));
    }

    SECTION("Constructor from const char* initializes static string correctly")
    {
        String<char, 5u> string("test");

        REQUIRE(4u == string.get_length());
        REQUIRE(5u == string.get_capacity());
        REQUIRE(0 == std::strncmp("test", string.get_cstring(), string.get_length()));
    }
}
TEST_CASE("String<char, N>: push_back behavior in string", "[lx][containers][String<char, N>]")
{
    using namespace lx::containers;

    SECTION("push_back(char) on empty string succeeds")
    {
        String<char, 5u> string;

        bool r = string.push_back('X');

        REQUIRE(true == r);
        REQUIRE(1u == string.get_length());
        REQUIRE(5u == string.get_capacity());
        REQUIRE(0 == std::strncmp("X", string.get_cstring(), string.get_length()));
    }

    SECTION("Multiple push_back(char) calls succeed until capacity is reached")
    {
        String<char, 5u> string;

        std::uint32_t r = 0;

        r += true == string.push_back('X') ? 1 : 0;
        r += true == string.push_back('Y') ? 1 : 0;
        r += true == string.push_back('Z') ? 1 : 0;

        REQUIRE(3u == r);
        REQUIRE(3u == string.get_length());
        REQUIRE(5u == string.get_capacity());
        REQUIRE(0 == std::strncmp("XYZ", string.get_cstring(), string.get_length()));
    }

    SECTION("push_back(char) fails when static string has no room")
    {
        String<char, 1u> string;

        bool r = string.push_back('X');

        REQUIRE(false == r);
        REQUIRE(0u == string.get_length());
        REQUIRE(1u == string.get_capacity());
    }

    SECTION("push_back(char) stops when capacity limit is exceeded")
    {
        String<char, 3u> string;

        std::uint32_t r = 0;

        r += true == string.push_back('X') ? 1 : 0;
        r += true == string.push_back('Y') ? 1 : 0;
        r += true == string.push_back('Z') ? 1 : 0;

        REQUIRE(2u == r);
        REQUIRE(2u == string.get_length());
        REQUIRE(3u == string.get_capacity());
        REQUIRE(0 == std::strncmp("XY", string.get_cstring(), string.get_length()));
    }
}
TEST_CASE("String<char, N>: conversion to string_view", "[lx][containers][String<char, N>]")
{
    using namespace lx::containers;

    String<char, 5u> test("test");

    std::string_view view = test;

    REQUIRE(4u == view.size());
    REQUIRE("test" == view);
}