#pragma once

/*
 *   Name: entry_point.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <string_view>
#include <vector>

// lxf
#include <lxf/common/scalar.hpp>
#include <lxf/device.hpp>

namespace lxf {
class Windower;

common::Int64 entry_point(std::string_view params_a,
                          const std::vector<device::GPU>& gpus_a,
                          const std::vector<device::Display>& displays_a,
                          Windower* p_windower_a);
} // namespace lxf