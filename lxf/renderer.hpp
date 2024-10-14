#pragma once

/*
 *   Name: renderer.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lxf
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/scalar.hpp>
#include <lxf/device.hpp>

namespace lxf {
struct renderer : private common::non_constructible
{
    class Context
    {
    public:
        Context(const device::GPU& gpu_a, common::Uint32 queue_family_index, device::GPU::Feature features_a);
    };
};
} // namespace lxf