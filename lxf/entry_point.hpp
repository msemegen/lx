#pragma once

/*
 *   Name: entry_point.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <string_view>
#include <vector>

// lxf
#include <lxf/device.hpp>

namespace lxf {
class Windower;

std::int64_t entry_point(std::string_view params_a,
                         std::span<const device::GPU*> gpus_a,
                         std::span<const device::Display*> displays_a,
                         Windower* p_windower_a);
} // namespace lxf