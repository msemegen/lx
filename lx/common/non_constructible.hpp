#pragma once

/*
 *   Name: non_constructible.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

namespace lx::common {

/// @brief A syntactic sugar class that makes the derived class non-constructible
class non_constructible
{
protected:
    non_constructible() = delete;
    non_constructible(const non_constructible&) = delete;
    non_constructible(non_constructible&&) = delete;

    non_constructible& operator=(const non_constructible&) = delete;
    non_constructible& operator=(non_constructible&&) = delete;
};
} // namespace lx::common