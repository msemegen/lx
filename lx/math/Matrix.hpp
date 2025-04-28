// lx
#include <lx/common/non_constructible.hpp>
#include <lx/math/Vector.hpp>

// std
#include <bit>
#include <cassert>
#include <cstddef>

namespace lx::math {
template<typename Type, std::size_t dimmensions> struct Matrix : private common::non_constructible
{
};

template<typename Type> struct Matrix<Type, 2u>
{
    Vector<Type, 2u> column_0;
    Vector<Type, 2u> column_1;

    const Vector<Type, 2u>& operator[](std::size_t index_a) const
    {
        assert(index_a < 2u);
        return std::bit_cast<Vector<Type, 2u>*>(this)[index_a];
    }
    Vector<Type, 2u>& operator[](std::size_t index_a)
    {
        assert(index_a < 2u);
        return std::bit_cast<Vector<Type, 2u>*>(this)[index_a];
    }
};
template<typename Type> struct Matrix<Type, 3u>
{
    Vector<Type, 3u> column_0;
    Vector<Type, 3u> column_1;
    Vector<Type, 3u> column_2;

    const Vector<Type, 3u>& operator[](std::size_t index_a) const
    {
        assert(index_a < 3u);
        return std::bit_cast<Vector<Type, 3u>*>(this)[index_a];
    }
    Vector<Type, 3u>& operator[](std::size_t index_a)
    {
        assert(index_a < 3u);
        return std::bit_cast<Vector<Type, 3u>*>(this)[index_a];
    }
};
template<typename Type> struct Matrix<Type, 4u>
{
    Vector<Type, 4u> column_0;
    Vector<Type, 4u> column_1;
    Vector<Type, 4u> column_2;
    Vector<Type, 4u> column_3;

    const Vector<Type, 4u>& operator[](std::size_t index_a) const
    {
        assert(index_a < 4u);
        return std::bit_cast<Vector<Type, 4u>*>(this)[index_a];
    }
    Vector<Type, 4u>& operator[](std::size_t index_a)
    {
        assert(index_a < 4u);
        return std::bit_cast<Vector<Type, 4u>*>(this)[index_a];
    }
};
} // namespace lx::math