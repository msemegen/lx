#pragma once

/*
 *   Name: Out.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <utility>

namespace lxf {
namespace common {
template<typename Type> class Out
{
public:
    Out() = delete;
    Out(const Type&) = delete;
    Out(Type&& param_a)
        : p(std::move(param_a))
    {
    }

    Out(Type& a)
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

template<typename Type> Out<Type> out(Type& arg_a)
{
    return std::move(Out<Type>(arg_a));
}
} // namespace common
} // namespace lxf