#pragma once

/*
 *   Name: Limited.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>

namespace lxf {
namespace common {
template<typename Type, Type minimum, Type maximum> class Limited
{
    static_assert(maximum >= minimum);

public:
    constexpr Limited()
        : value(minimum)
    {
    }

    Limited(const Limited& other)
        : value(other.value)
    {
    }

    Limited(Limited&& other)
        : value(other.value)
    {
    }

    Limited(Type value)
        : value(value)
    {
        assert(this->value >= minimum && this->value <= maximum);
    }

    Limited& operator=(const Limited& other)
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
} // namespace common
} // namespace lxf
