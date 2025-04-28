// external
#include <catch2/catch_test_macros.hpp>

// lx
#include <lx/common/Position.hpp>

TEST_CASE("Position<T, 2u> works properly", "[lx][common][Position<T, 2u>]")
{
    using namespace lx::common;

    Position<std::int32_t, 2u> _2d_pos;

    SECTION("Default fields values are 0")
    {
        REQUIRE(0 == _2d_pos.x);
        REQUIRE(0 == _2d_pos.y);

        REQUIRE(0 == _2d_pos[0u]);
        REQUIRE(0 == _2d_pos[1u]);
    }

    SECTION("Fields are setable using x/y")
    {
        _2d_pos.x = 1;
        _2d_pos.y = 2;

        REQUIRE(1 == _2d_pos.x);
        REQUIRE(2 == _2d_pos.y);

        REQUIRE(1 == _2d_pos[0u]);
        REQUIRE(2 == _2d_pos[1u]);
    }

    SECTION("Fields are setable using operator[]")
    {
        _2d_pos[0u] = 3;
        _2d_pos[1u] = 4;

        REQUIRE(3 == _2d_pos.x);
        REQUIRE(4 == _2d_pos.y);

        REQUIRE(3 == _2d_pos[0u]);
        REQUIRE(4 == _2d_pos[1u]);
    }
}
TEST_CASE("Position<T, 3u> works properly", "[lx][common][Position<T, 3u>]")
{
    using namespace lx::common;

    Position<std::int32_t, 3u> _3d_pos;

    SECTION("Default fields values are 0")
    {
        REQUIRE(0 == _3d_pos.x);
        REQUIRE(0 == _3d_pos.y);
        REQUIRE(0 == _3d_pos.z);

        REQUIRE(0 == _3d_pos[0u]);
        REQUIRE(0 == _3d_pos[1u]);
        REQUIRE(0 == _3d_pos[2u]);
    }

    SECTION("Fields are setable using x/y")
    {
        _3d_pos.x = 1;
        _3d_pos.y = 2;
        _3d_pos.z = 3;

        REQUIRE(1 == _3d_pos.x);
        REQUIRE(2 == _3d_pos.y);
        REQUIRE(3 == _3d_pos.z);

        REQUIRE(1 == _3d_pos[0u]);
        REQUIRE(2 == _3d_pos[1u]);
        REQUIRE(3 == _3d_pos[2u]);
    }

    SECTION("Fields are setable using operator[]")
    {
        _3d_pos[0u] = 4;
        _3d_pos[1u] = 5;
        _3d_pos[2u] = 6;

        REQUIRE(4 == _3d_pos.x);
        REQUIRE(5 == _3d_pos.y);
        REQUIRE(6 == _3d_pos.z);

        REQUIRE(4 == _3d_pos[0u]);
        REQUIRE(5 == _3d_pos[1u]);
        REQUIRE(6 == _3d_pos[2u]);
    }
}