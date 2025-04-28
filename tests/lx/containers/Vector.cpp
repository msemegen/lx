// external
#include <catch2/catch_test_macros.hpp>

// lx
#include <lx/containers/Vector.hpp>

TEST_CASE("Vector<T>: construction", "[lx][containers][Vector<T>]")
{
    using namespace lx::containers;

    SECTION("Default constructor creates empty vector")
    {
        Vector<std::byte> vector;

        REQUIRE(0u == vector.get_length());
        REQUIRE(0u == vector.get_capacity());
        REQUIRE(true == vector.is_empty());
    }

    SECTION("Constructor with reserved capacity initializes vector correctly")
    {
        Vector<std::byte> vector(5u);

        REQUIRE(0u == vector.get_length());
        REQUIRE(5u == vector.get_capacity());
        REQUIRE(true == vector.is_empty());
    }

    SECTION("Initializer list constructor fills vector with values")
    {
        Vector<std::byte> vector({ std::byte { 0x1u }, std::byte { 0x2u }, std::byte { 0x3u } });

        REQUIRE(3u == vector.get_length());
        REQUIRE(3u == vector.get_capacity());
        REQUIRE(false == vector.is_empty());

        std::span<const std::byte> span = vector;

        REQUIRE(std::byte { 0x1u } == span[0]);
        REQUIRE(std::byte { 0x2u } == span[1]);
        REQUIRE(std::byte { 0x3u } == span[2]);
    }

    SECTION("Span constructor copies elements from input span")
    {
        const std::byte arr[] = { std::byte { 0x1u }, std::byte { 0x2u }, std::byte { 0x3u } };
        Vector<std::byte> vector(std::span { arr });

        REQUIRE(3u == vector.get_length());
        REQUIRE(3u == vector.get_capacity());
        REQUIRE(false == vector.is_empty());

        std::span<const std::byte> span = vector;

        REQUIRE(std::byte { 0x1u } == span[0]);
        REQUIRE(std::byte { 0x2u } == span[1]);
        REQUIRE(std::byte { 0x3u } == span[2]);
    }

    SECTION("Copy constructor creates identical vector")
    {
        const std::byte arr[] = { std::byte { 0x1u }, std::byte { 0x2u }, std::byte { 0x3u } };
        Vector<std::byte> vector_1(std::span { arr });
        Vector<std::byte> vector_2(vector_1);

        std::span<const std::byte> vector_1_span = vector_1;
        std::span<const std::byte> vector_2_span = vector_2;

        REQUIRE(vector_1_span[0] == vector_2_span[0]);
        REQUIRE(vector_1_span[1] == vector_2_span[1]);
        REQUIRE(vector_1_span[2] == vector_2_span[2]);
    }

    SECTION("Move constructor transfers ownership and leaves source empty")
    {
        const std::byte arr[] = { std::byte { 0x1u }, std::byte { 0x2u }, std::byte { 0x3u } };
        Vector<std::byte> source(std::span { arr });

        REQUIRE(3u == source.get_length());

        Vector<std::byte> moved(std::move(source));

        REQUIRE(3u == moved.get_length());

        std::span<const std::byte> span = moved;
        REQUIRE(span[0] == std::byte { 0x1u });
        REQUIRE(span[1] == std::byte { 0x2u });
        REQUIRE(span[2] == std::byte { 0x3u });

        REQUIRE(0u == source.get_length());
    }
}