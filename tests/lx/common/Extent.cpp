// externals
#include <catch2/catch_test_macros.hpp>

// lx
#include <lx/common/Extent.hpp>

// std
#include <cstdint>

TEST_CASE("Extent<T, 2u>", "[lx][common][Extent]")
{
    using namespace lx::common;

    Extent<std::int32_t, 2u> extent;

    SECTION("Default fields values are 0")
    {
        REQUIRE(0 == extent.w);
        REQUIRE(0 == extent.h);

        REQUIRE(0 == extent[0u]);
        REQUIRE(0 == extent[1u]);
    }

    SECTION("Fields are settable via w/h")
    {
        extent.w = 1;
        extent.h = 2;

        REQUIRE(1 == extent.w);
        REQUIRE(2 == extent.h);

        REQUIRE(1 == extent[0u]);
        REQUIRE(2 == extent[1u]);
    }

    SECTION("Fields are settable using operator[]")
    {
        extent[0] = 3;
        extent[1] = 4;

        REQUIRE(3 == extent.w);
        REQUIRE(4 == extent.h);
    }
}
