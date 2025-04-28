// external
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// lx
#include <lx/math/Vector.hpp>

// std
#include <cstdint>

TEST_CASE("Vector<T, 2u>: construction", "[lx][common][math][Vector<T, 2u>]")
{
    using namespace lx::math;

    SECTION("Default constructor creates empty vector")
    {
        Vector<std::int32_t, 2u> vec;

        REQUIRE(0 == vec.x);
        REQUIRE(0 == vec.y);

        REQUIRE(0 == vec[0]);
        REQUIRE(0 == vec[1]);
    }
}
TEST_CASE("Vector<T, 2u>: unit vectors", "[lx][common][math][Vector<T, 2u>]")
{
    using namespace lx::math;

    SECTION("Vector<T, 2u>::unit_x should be a vector with x = 1 and y = 0")
    {
        Vector<std::int32_t, 2u> vec = Vector<std::int32_t, 2u>::unit_x;

        REQUIRE(1 == vec.x);
        REQUIRE(0 == vec.y);

        REQUIRE(1 == vec[0]);
        REQUIRE(0 == vec[1]);
    }
    SECTION("Vector<T, 2u>::unit_y should be a vector with x = 0 and y = 1")
    {
        Vector<std::int32_t, 2u> vec = Vector<std::int32_t, 2u>::unit_y;

        REQUIRE(0 == vec.x);
        REQUIRE(1 == vec.y);

        REQUIRE(0 == vec[0]);
        REQUIRE(1 == vec[1]);
    }
}
TEST_CASE("Vector<T, 2u>: operators", "[lx][common][math][Vector<T, 2u>]")
{
    using namespace lx::math;

    SECTION("operator- negates vector fields")
    {
        constexpr Vector<std::int32_t, 2u> vec { .x = 1, .y = 2 };
        constexpr auto vec_2 = -vec;

        REQUIRE(vec_2.x == -vec.x);
        REQUIRE(vec_2.y == -vec.y);
    }
    SECTION("operator[] can access x/y fields")
    {
        Vector<std::int32_t, 2u> vec { .x = 10, .y = 20 };

        REQUIRE(vec.x == vec[0]);
        REQUIRE(vec.y == vec[1]);
    }
    SECTION("operator[] can modify x/y fields")
    {
        Vector<std::int32_t, 2u> vec { .x = 10, .y = 20 };

        vec[0] = 12;
        vec[1] = 13;

        REQUIRE(12 == vec[0]);
        REQUIRE(13 == vec[1]);
    }
    SECTION("operator + can add two vectors")
    {
        constexpr Vector<std::int32_t, 2u> vec_1 { .x = 10, .y = 20 };
        constexpr Vector<std::int32_t, 2u> vec_2 { .x = 12, .y = 22 };

        constexpr auto vec_3 = vec_1 + vec_2;

        REQUIRE(vec_3.x == vec_1.x + vec_2.x);
        REQUIRE(vec_3.y == vec_1.y + vec_2.y);
    }
    SECTION("operator - can substract two vectors")
    {
        constexpr Vector<std::int32_t, 2u> vec_1 { .x = 20, .y = 40 };
        constexpr Vector<std::int32_t, 2u> vec_2 { .x = 12, .y = 22 };

        constexpr auto vec_3 = vec_1 - vec_2;

        REQUIRE(vec_3.x == vec_1.x - vec_2.x);
        REQUIRE(vec_3.y == vec_1.y - vec_2.y);
    }
    SECTION("operator * can multiply vector by scalar")
    {
        constexpr Vector<std::int32_t, 2u> vec_1 { .x = 20, .y = 40 };
        constexpr std::int32_t k = 2;

        constexpr auto vec_2 = vec_1 * k;
        constexpr auto vec_3 = k * vec_1;

        REQUIRE(vec_2.x == vec_1.x * k);
        REQUIRE(vec_2.y == vec_1.y * k);

        REQUIRE(vec_3.x == vec_1.x * k);
        REQUIRE(vec_3.y == vec_1.y * k);
    }
    SECTION("operator / can divde vector by scalar") {}
    // operator ==
    // operator !=
    // operator >
    // operator >=
    // operator <
    // operator <=
}
TEST_CASE("Vector<T, 2u>: functions", "[lx][common][math][Vector<T, 2u>]")
{
    using namespace lx::common;
    using namespace lx::math;

    SECTION("zero function zeroes vector")
    {
        Vector<std::int32_t, 2u> vec { .x = 1, .y = 2 };

        zero(out(vec));

        REQUIRE(0 == vec.x);
        REQUIRE(0 == vec.y);

        REQUIRE(0 == vec[0]);
        REQUIRE(0 == vec[1]);
    }
    SECTION("length_squared function calculates squared vector length")
    {
        constexpr Vector<std::int32_t, 2u> vec { .x = 1, .y = 2 };
        constexpr std::int32_t sq = length_squared(vec);

        REQUIRE(5 == sq);
    }
    SECTION("length function calculates vector length")
    {
        constexpr Vector<std::int32_t, 2u> vec { .x = 1, .y = 2 };
        const std::int32_t l = length(vec);

        REQUIRE(2 == l);
    }
    SECTION("normalize function normalizes vector length")
    {
        Vector<float, 2u> vec { 1.0f, 2.0f };

        normalize(out(vec));

        REQUIRE(Catch::Matchers::WithinRel(length(vec)).match(1.0f));
    }
    SECTION("normalizeed function returns normalized vector")
    {
        Vector<float, 2u> vec { 1.0f, 2.0f };

        vec = normalized(vec);

        REQUIRE(Catch::Matchers::WithinRel(length(vec)).match(1.0f));
    }
}