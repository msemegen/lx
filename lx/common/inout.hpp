#pragma once

/*
 *   Name: inout.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <utility>

namespace lx::common {

/// @brief A syntactic sugar type that indicates that the function parameter is an input/output parameter.
template<typename Type> class inout
{
public:
    inout() = delete;
    inout(const Type&) = delete;
    explicit inout(Type&& param)
        : p(std::move(param))
    {
    }

    explicit inout(Type& a)
        : p(a)
    {
    }

    Type& operator*() const
    {
        return this->p;
    }

    Type* operator->() const
    {
        return &(this->p);
    }

private:
    Type& p;
};
} // namespace lx::common