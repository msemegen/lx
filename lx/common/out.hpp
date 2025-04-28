#pragma once

/*
 *   Name: out.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <utility>

namespace lx::common {

/// @brief A syntactic sugar type that indicates that the function parameter is an output parameter.
template<typename Type> class out
{
public:
    out() = delete;
    out(const Type&) = delete;
    explicit out(Type&& param)
        : p(std::move(param))
    {
    }

    explicit out(Type& a)
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