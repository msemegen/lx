#pragma once

/*
 *   Name: Out.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <utility>

namespace lxf::common {
template<typename Type> class Out
{
public:
    Out() = delete;
    Out(const Type&) = delete;
    Out(Type&& param)
        : p(std::move(param))
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

template<typename Type> Out<Type> out(Type& arg)
{
    return std::move(Out<Type>(arg));
}
} // namespace lxf::common