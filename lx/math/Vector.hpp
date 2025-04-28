#pragma once

// lx
#include <lx/common/non_constructible.hpp>
#include <lx/common/out.hpp>
#include <lx/math/tools.hpp>

// std
#include <bit>
#include <cassert>
#include <cstddef>

namespace lx::math {
template<typename Type, std::size_t dimmensions> struct Vector : private common::non_constructible
{
};

template<typename Type> struct Vector<Type, 2u>
{
    Type x = static_cast<Type>(0);
    Type y = static_cast<Type>(0);

    [[nodiscard]] Type operator[](std::size_t index_a) const
    {
        assert(index_a < 2u);

        return std::bit_cast<Type*>(this)[index_a];
    }
    [[nodiscard]] Type& operator[](std::size_t index_a)
    {
        assert(index_a < 2u);

        return std::bit_cast<Type*>(this)[index_a];
    }

    constexpr [[nodiscard]] Vector<Type, 2u> operator-() const
    {
        return { .x = -this->x, .y = -this->y };
    }
    constexpr [[nodiscard]] Vector<Type, 2u> operator+() const
    {
        return { .x = +this->x, .y = +this->y };
    }

    const static Vector<Type, 2u> unit_x;
    const static Vector<Type, 2u> unit_y;
};

template<typename Type> const Vector<Type, 2u> Vector<Type, 2u>::unit_x = { static_cast<Type>(1), static_cast<Type>(0) };
template<typename Type> const Vector<Type, 2u> Vector<Type, 2u>::unit_y = { static_cast<Type>(0), static_cast<Type>(1) };

template<typename Type> [[nodiscard]] bool operator==(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return tools::is_equal(left_a.x, right_a.x) && tools::is_equal(left_a.y, right_a.y);
}
template<typename Type> [[nodiscard]] bool operator!=(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return false == (left_a == right_a);
}

template<typename Type> [[nodiscard]] bool operator>(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return left_a.x > right_a.x && left_a.y > right_a.y;
}
template<typename Type> [[nodiscard]] bool operator>=(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return left_a.x >= right_a.x && left_a.y >= right_a.y;
}

template<typename Type> [[nodiscard]] bool operator<(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return left_a.x < right_a.x && left_a.y < right_a.y;
}
template<typename Type> [[nodiscard]] bool operator<=(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return left_a.x <= right_a.x && left_a.y <= right_a.y;
}

template<typename Type> constexpr [[nodiscard]] Vector<Type, 2u> operator+(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return { .x = left_a.x + right_a.x, .y = left_a.y + right_a.y };
}
template<typename Type> constexpr [[nodiscard]] Vector<Type, 2u> operator-(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return { .x = left_a.x - right_a.x, .y = left_a.y - right_a.y };
}

template<typename Type> constexpr [[nodiscard]] Vector<Type, 2u> operator*(Vector<Type, 2u> left_a, Type right_a)
{
    return { .x = left_a.x * right_a, .y = left_a.y * right_a };
}
template<typename Type> constexpr [[nodiscard]] Vector<Type, 2u> operator*(Type left_a, Vector<Type, 2u> right_a)
{
    return right_a * left_a;
}

template<typename Type> constexpr [[nodiscard]] Vector<Type, 2u> operator/(Vector<Type, 2u> left_a, Type right_a)
{
    return { .x = left_a.x / right_a, .y = left_a.y / right_a };
}

template<typename Type> constexpr void zero(lx::common::out<Vector<Type, 2u>> vec_a)
{
    vec_a->x = static_cast<Type>(0);
    vec_a->y = static_cast<Type>(0);
}
template<typename Type> constexpr [[nodiscard]] Type zeroed()
{
    return {};
}
template<typename Type> constexpr [[nodiscard]] bool is_zero(const Vector<Type, 2u>& vector_a)
{
    return Vector<Type, 2u> { .x = static_cast<Type>(0), .y = static_cast<Type>(0) } == vector_a;
}

template<typename Type> constexpr [[nodiscard]] Type length_squared(Vector<Type, 2u> vector_a)
{
    return vector_a.x * vector_a.x + vector_a.y * vector_a.y;
}
template<typename Type> constexpr Type [[nodiscard]] length(Vector<Type, 2u> vector_a)
{
    Type sq = length_squared(vector_a);
    return static_cast<Type>(std::sqrt(sq));
}

template<typename Type> constexpr void normalize(lx::common::out<Vector<Type, 2u>> vector_a)
{
    Type l = length(*vector_a);

    vector_a->x = vector_a->x / l;
    vector_a->y = vector_a->y / l;
}
template<typename Type> constexpr Vector<Type, 2u> normalized(Vector<Type, 2u> vector_a)
{
    Type l = length(vector_a);
    return { .x = vector_a.x / l, .y = vector_a.y / l };
}
template<typename Type> constexpr bool is_normalized(Vector<Type, 2u> vector_a)
{
    return tools::is_equal(1.0f, length(vector_a));
}

template<typename Type> constexpr Type dot(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return left_a.x * right_a.x + left_a.y * right_a.y;
}
template<typename Type> constexpr Type cross(Vector<Type, 2u> left_a, Vector<Type, 2u> right_a)
{
    return left_a.x * right_a.y - left_a.y * right_a.x;
}

template<typename Type> Vector<Type, 2u> lerp(Vector<Type, 2u> start_a, const Vector<Type, 2u> end_a, float step_a)
{
    return start_a * (1.0f - step_a) + end_a * step_a;
}

template<typename Type> struct Vector<Type, 3u>
{
    Type x = static_cast<Type>(0);
    Type y = static_cast<Type>(0);
    Type z = static_cast<Type>(0);

    [[nodiscard]] Type operator[](std::size_t index_a) const
    {
        assert(index_a < 3u);

        return std::bit_cast<Type*>(this)[index_a];
    }
    [[nodiscard]] Type& operator[](std::size_t index_a)
    {
        assert(index_a < 3u);

        return std::bit_cast<Type*>(this)[index_a];
    }

    [[nodiscard]] constexpr Vector<Type, 2u> operator-() const
    {
        return { .x = -this->x, .y = -this->y, .z = -this->z };
    }
    [[nodiscard]] constexpr Vector<Type, 2u> operator+() const
    {
        return { .x = +this->x, .y = +this->y, .z = +this->z };
    }
};
template<typename Type> struct Vector<Type, 4u>
{
    Type x = static_cast<Type>(0);
    Type y = static_cast<Type>(0);
    Type z = static_cast<Type>(0);
    Type w = static_cast<Type>(0);

    [[nodiscard]] Type operator[](std::size_t index_a) const
    {
        assert(index_a < 4u);

        return std::bit_cast<Type*>(this)[index_a];
    }
    [[nodiscard]] Type& operator[](std::size_t index_a)
    {
        assert(index_a < 4u);

        return std::bit_cast<Type*>(this)[index_a];
    }

    [[nodiscard]] constexpr Vector<Type, 2u> operator-() const
    {
        return { .x = -this->x, .y = -this->y, .z = -this->z, .w = -this->w };
    }
    [[nodiscard]] constexpr Vector<Type, 2u> operator+() const
    {
        return { .x = +this->x, .y = +this->y, .z = +this->z, .w = -this->w };
    }
};

template<typename Type> void zero(lx::common::out<Vector<Type, 3u>> vec_a)
{
    vec_a->x = static_cast<Type>(0);
    vec_a->y = static_cast<Type>(0);
    vec_a->z = static_cast<Type>(0);
}
template<typename Type> void zero(lx::common::out<Vector<Type, 4u>> vec_a)
{
    vec_a->x = static_cast<Type>(0);
    vec_a->y = static_cast<Type>(0);
    vec_a->z = static_cast<Type>(0);
    vec_a->w = static_cast<Type>(0);
}

template<typename Type> constexpr [[nodiscard]] Type length_squared(Vector<Type, 3u> vector_a)
{
    return vector_a.x * vector_a.x + vector_a.y * vector_a.y + vector_a.z * vector_a.z;
}
template<typename Type> constexpr [[nodiscard]] Type length_squared(Vector<Type, 4u> vector_a)
{
    return vector_a.x * vector_a.x + vector_a.y * vector_a.y + vector_a.z * vector_a.z + vector_a.w * vector_a.w;
}

template<typename Type> constexpr Type [[nodiscard]] length(Vector<Type, 3u> vector_a)
{
    Type sq = length_squared(vector_a);
    return static_cast<Type>(std::sqrt(sq));
}
template<typename Type> constexpr Type [[nodiscard]] length(Vector<Type, 4u> vector_a)
{
    Type sq = length_squared(vector_a);
    return static_cast<Type>(std::sqrt(sq));
}
} // namespace lx::math