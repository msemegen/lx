#pragma once

/*
 *   Name: scalar.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

namespace lxf::common {
#if defined(LX_AMD64)
static_assert(sizeof(float) == 4u);
static_assert(sizeof(double) == 8u);
using Float32 = float;
using Float64 = double;

static_assert(sizeof(unsigned char) == 1u && sizeof(char) == 1u);
static_assert(sizeof(unsigned short int) == 2u && sizeof(short int) == 2u);
static_assert(sizeof(unsigned long int) == 4u && sizeof(long int) == 4u);
static_assert(sizeof(unsigned long long int) == 8u && sizeof(long long int) == 8u);

using Byte = unsigned char;

using Uint8 = unsigned char;
using Uint16 = unsigned short int;
using Uint32 = unsigned long int;
using Uint64 = unsigned long long int;

using Int8 = char;
using Int16 = short int;
using Int32 = long int;
using Int64 = long long int;
#endif
} // namespace lxf::common