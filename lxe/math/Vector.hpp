#pragma once

/*
 *   Name: Vector_2.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <bit>
#include <cassert>

// lxf
#include <lxf/common/Out.hpp>
#include <lxf/common/non_constructible.hpp>

namespace lxe {
namespace math {

template<typename Scalar, std::size_t dimmensions> struct Vector : private lxf::common::non_constructible
{
};

template<typename Scalar> struct Vector<Scalar, 2u>
{
    Scalar x = static_cast<Scalar>(0);
    Scalar y = static_cast<Scalar>(0);

    Scalar operator[](std::size_t index) const
    {
        assert(index <= 2);

        return std::bit_cast<const Scalar*>(this)[index];
    }

    Scalar& operator[](std::size_t index)
    {
        assert(index <= 2);

        return std::bit_cast<Scalar*>(this)[index];
    }
};

template<typename Scalar> Vector<Scalar, 2u> operator+(Vector<Scalar, 2u> left, Vector<Scalar, 2u> right)
{
    return { .x = left.x + right.x, .y = left.y + right.y };
}
template<typename Scalar> Vector<Scalar, 2u> operator-(Vector<Scalar, 2u> left, Vector<Scalar, 2u> right)
{
    return { .x = left.x - right.x, .y = left.y - right.y };
}
template<typename Scalar> Vector<Scalar, 2u> operator*(Vector<Scalar, 2u> left, Scalar right)
{
    return { .x = left.x * right, .y = left.y * right };
}
template<typename Scalar> Vector<Scalar, 2u> operator*(Scalar left, Vector<Scalar, 2u> right)
{
    return { .x = left * right.x, .y = left * right.y };
}

template<typename Scalar> constexpr void zero(lxf::common::Out<Vector<Scalar, 2u>> vector)
{
    vector->x = static_cast<Scalar>(0);
    vector->y = static_cast<Scalar>(0);
}
template<typename Scalar> constexpr void abs(lxf::common::Out<Vector<Scalar, 2u>> vector)
{
    vector->x = std::abs(vector->x);
    vector->y = std::abs(vector->y);
}
template<typename Scalar> constexpr Scalar length_squared(Vector<Scalar, 2u> vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}
template<typename Scalar> constexpr Scalar length(Vector<Scalar, 2u> vector)
{
    return std::sqrt(length_squared(vector));
}

template<typename Scalar> struct Vector<Scalar, 3u>
{
    Scalar x = static_cast<Scalar>(0);
    Scalar y = static_cast<Scalar>(0);
    Scalar z = static_cast<Scalar>(0);

    Scalar operator[](std::size_t index) const
    {
        assert(index <= 3);

        return std::bit_cast<const Scalar*>(this)[index];
    }

    Scalar& operator[](std::size_t index)
    {
        assert(index <= 3);

        return std::bit_cast<Scalar*>(this)[index];
    }

    operator Vector<Scalar, 2u>() const
    {
        return Vector<Scalar, 2u> { .x = x, .y = y };
    }
};

template<typename Scalar> Vector<Scalar, 3u> operator+(Vector<Scalar, 3u> left, Vector<Scalar, 3u> right)
{
    return { .x = left.x + right.x, .y = left.y + right.y, .z = left.z + right.z };
}
template<typename Scalar> Vector<Scalar, 3u> operator-(Vector<Scalar, 3u> left, Vector<Scalar, 3u> right)
{
    return { .x = left.x - right.x, .y = left.y - right.y, .z = left.z - right.z };
}
template<typename Scalar> Vector<Scalar, 3u> operator*(Vector<Scalar, 3u> left, Scalar right)
{
    return { .x = left.x * right, .y = left.y * right, .z = left.z * right };
}
template<typename Scalar> Vector<Scalar, 3u> operator*(Scalar left, Vector<Scalar, 3u> right)
{
    return { .x = left * right.x, .y = left * right.y, .z = left * right.z };
}

template<typename Scalar> constexpr void zero(lxf::common::Out<Vector<Scalar, 3u>> vector)
{
    vector->x = static_cast<Scalar>(0);
    vector->y = static_cast<Scalar>(0);
    vector->z = static_cast<Scalar>(0);
}
template<typename Scalar> constexpr void abs(lxf::common::Out<Vector<Scalar, 3u>> vector)
{
    vector->x = std::abs(vector->x);
    vector->y = std::abs(vector->y);
    vector->z = std::abs(vector->z);
}
template<typename Scalar> constexpr Scalar length_squared(Vector<Scalar, 3u> vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}
template<typename Scalar> constexpr Scalar length(Vector<Scalar, 3u> vector)
{
    return std::sqrt(length_squared(vector));
}
} // namespace math
} // namespace lxe