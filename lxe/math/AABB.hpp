#pragma once

/*
 *   Name: AABB.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lxf
#include <lxf/common/Out.hpp>
#include <lxf/common/non_constructible.hpp>

// lxe
#include <lxe/math/Vector.hpp>

namespace lxe {
namespace math {
template<typename Scalar, std::size_t dimmensions> struct AABB : private lxf::common::non_constructible
{
};

template<typename Scalar> struct AABB<Scalar, 2u>
{
    Vector<Scalar, 2u> left_lower;
    Vector<Scalar, 2u> right_upper;
};

template<typename Scalar> Scalar perimeter(AABB<Scalar, 2u> aabb)
{
    const Scalar a = aabb.right_upper.x - aabb.left_lower.x;
    const Scalar b = aabb.right_upper.y - aabb.left_lower.y;

    return Scalar(2) * (a + b);
}
template<typename Scalar> Scalar area(AABB<Scalar, 2u> aabb)
{
    const Scalar a = aabb.right_upper.x - aabb.left_lower.x;
    const Scalar b = aabb.right_upper.y - aabb.left_lower.y;

    return a * b;
}

template<typename Scalar> bool contains(AABB<Scalar, 2u> outer, AABB<Scalar, 2u> inner)
{
    bool ret = true;

    ret = ret && outer.left_lower.x <= inner.left_lower.x;
    ret = ret && outer.left_lower.y <= inner.left_lower.y;
    ret = ret && inner.right_upper.x <= outer.right_upper.x;
    ret = ret && inner.right_upper.y <= outer.right_upper.y;

    return ret;
}

template<typename Scalar> bool overlaps(AABB<Scalar, 2u> left, AABB<Scalar, 2u> right)
{
    return {};
}
template<typename Scalar> bool enlarge(lxf::common::Out<AABB<Scalar, 2u>> out, AABB<Scalar, 2u> right)
{
    return {};
}

template<typename Scalar> Vector<Scalar, 2u> get_center(AABB<Scalar, 2u> aabb)
{
    return { .x = (aabb.right_upper.x + aabb.left_lower.x) / static_cast<Scalar>(2),
             .y = (aabb.right_upper.y + aabb.left_lower.y) / static_cast<Scalar>(2) };
}

} // namespace math
} // namespace lxe