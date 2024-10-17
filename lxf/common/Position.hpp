#pragma once

/*
 *   Name: Position.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

namespace lxf::common {
template<typename Scalar_t> struct Position
{
    Scalar_t x = static_cast<Scalar_t>(0);
    Scalar_t y = static_cast<Scalar_t>(0);
};
} // namespace lxf::common