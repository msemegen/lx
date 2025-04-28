#pragma once

/*
 *   Name: ranged.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>

namespace lx::common {

/// @brief A syntactic sugar class that represents a value within a range.
/// @tparam Type of the value.
/// @tparam minimum of the range.
/// @tparam maximum of the range.
template<typename Type, Type minimum, Type maximum> class ranged
{
    static_assert(maximum >= minimum);

public:
    constexpr ranged()
        : value(minimum)
    {
    }

    ranged(const ranged& other)
        : value(other.value)
    {
    }

    ranged(ranged&& other)
        : value(other.value)
    {
    }

    ranged(Type value)
        : value(value)
    {
        assert(this->value >= minimum && this->value <= maximum);
    }

    ranged& operator=(const ranged& other)
    {
        if (this != &(other))
        {
            this->value = other.value;
        }

        return *this;
    }

    operator Type()
    {
        return this->value;
    }
    operator Type() const
    {
        return this->value;
    }

    constexpr Type get() const
    {
        return this->value;
    }

private:
    Type value;
};
} // namespace lx::common