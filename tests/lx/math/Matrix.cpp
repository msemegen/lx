// external
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// lx
#include <lx/math/Matrix.hpp>

// std
#include <cstdint>

TEST_CASE("Matrix<T, 2u>: construction", "[lx][math][Matrix<T, 2u>]")
{
    using namespace lx::math;

    SECTION("Default constructor creates empty vector")
    {
        Matrix<std::int32_t, 2u> mat;

        REQUIRE(true == is_zero(mat.column_0));
        REQUIRE(true == is_zero(mat.column_1));
    }
}
