#pragma once

// lx
#include <lx/common/non_constructible.hpp>
#include <lx/math/Vector.hpp>

// std
#include <cstdlib>

namespace lx::math {
template<typename Type, std::size_t extent> struct AABB : private lx::common::non_constructible
{
};

template<typename Type> struct AABB<Type, 2u>
{
    lx::math::Vector<Type, 2u> lower_bound;
    lx::math::Vector<Type, 2u> upper_bound;
};

template<typename Type> struct AABB<Type, 3u>
{
    lx::math::Vector<Type, 3u> lower_bound;
    lx::math::Vector<Type, 3u> upper_bound;
};
} // namespace lx::math