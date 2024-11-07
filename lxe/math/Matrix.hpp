#pragma once

// std
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>

// lxf
#include <lxf/common/Out.hpp>
#include <lxf/common/non_constructible.hpp>

// lxe
#include <lxe/math/Vector.hpp>

namespace lxe {
namespace math {
template<typename Scalar, std::size_t rows, std::size_t columns> struct Matrix : private lxf::common::non_constructible
{
};

template<typename Scalar> struct Matrix<Scalar, 2, 2>
{
    Vector<Scalar, 2u> column_0;
    Vector<Scalar, 2u> column_1;

    Vector<Scalar, 2u> operator[](std::size_t column) const
    {
        assert(column < 2u);

        return std::bit_cast<const Vector<Scalar, 2u>*>(this)[column];
    }
    Vector<Scalar, 2u>& operator[](std::size_t column)
    {
        assert(column < 2u);

        return std::bit_cast<Vector<Scalar, 2u>*>(this)[column];
    }
};
template<typename Scalar> constexpr void zero(lxf::common::Out<Matrix<Scalar, 2u, 2u>> matrix)
{
    zero(lxf::common::out(matrix->column_0));
    zero(lxf::common::out(matrix->column_1));
}
template<typename Scalar> constexpr void eye(lxf::common::Out<Matrix<Scalar, 2u, 2u>> matrix)
{
    (*matrix)[0].x = static_cast<Scalar>(1);
    (*matrix)[0].y = static_cast<Scalar>(0);

    (*matrix)[1].x = static_cast<Scalar>(0);
    (*matrix)[1].y = static_cast<Scalar>(1);
}

template<typename Scalar> struct Matrix<Scalar, 3, 3>
{
    Vector<Scalar, 3u> column_0;
    Vector<Scalar, 3u> column_1;
    Vector<Scalar, 3u> column_2;

    Vector<Scalar, 3u> operator[](std::size_t column) const
    {
        assert(column < 3u);

        return std::bit_cast<const Vector<Scalar, 3u>*>(this)[column];
    }
    Vector<Scalar, 3u>& operator[](std::size_t column)
    {
        assert(column < 3u);

        return std::bit_cast<Vector<Scalar, 3u>*>(this)[column];
    }
};
template<typename Scalar> constexpr void zero(lxf::common::Out<Matrix<Scalar, 3u, 3u>> matrix)
{
    zero(lxf::common::out(matrix->column_0));
    zero(lxf::common::out(matrix->column_1));
    zero(lxf::common::out(matrix->column_2));
}
template<typename Scalar> constexpr void eye(lxf::common::Out<Matrix<Scalar, 3u, 3u>> matrix)
{
    (*matrix)[0].x = static_cast<Scalar>(1);
    (*matrix)[0].y = static_cast<Scalar>(0);
    (*matrix)[0].z = static_cast<Scalar>(0);

    (*matrix)[1].x = static_cast<Scalar>(0);
    (*matrix)[1].y = static_cast<Scalar>(1);
    (*matrix)[1].z = static_cast<Scalar>(0);

    (*matrix)[2].x = static_cast<Scalar>(0);
    (*matrix)[2].y = static_cast<Scalar>(0);
    (*matrix)[2].z = static_cast<Scalar>(1);
}
} // namespace math
} // namespace lxe