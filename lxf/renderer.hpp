#pragma once

/*
 *   Name: renderer.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// platform
#include <vulkan/vulkan.hpp>

// lxf
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/scalar.hpp>
#include <lxf/device.hpp>

namespace lxf {
struct renderer : private common::non_constructible
{
    class Context
    {
    private:
        Context(const device::GPU& gpu_a, const std::vector<device::GPU::Queue>& queues_a, device::GPU::Feature features_a);

        VkDevice vk_device;

        friend struct renderer;
    };

    template<typename Type_t> static [[nodiscard]] Context*
    create(const device::GPU& gpu_a, const std::vector<device::GPU::Queue>& queues_a, device::GPU::Feature features_a) = delete;
};
} // namespace lxf