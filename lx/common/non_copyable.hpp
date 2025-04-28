#pragma once

/*
 *   Name: non_copyable.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

namespace lx::common {

/// @brief A syntactic sugar class that makes the derived class non-copyable.
class non_copyable
{
public:
    non_copyable() = default;
    non_copyable(non_copyable&&) = default;
    non_copyable& operator=(non_copyable&&) = default;

protected:
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
};
} // namespace lx::common
