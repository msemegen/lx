/*
 *   Name: renderer.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/renderer.hpp>

// std
#include <memory>

// lxf
#include <lxf/common/bit.hpp>
#include <lxf/loader.hpp>

extern VkInstance vk_instance;

namespace lxf {
using namespace lxf::common;

bool renderer::Context::create(const device::GPU* p_gpu_a, const Properties& properties_a)
{
    const device::GPU::Properties gpu_properties = p_gpu_a->get_properties();

    const std::unique_ptr<VkDeviceQueueCreateInfo[]> vk_device_queues_create_info =
        std::make_unique<VkDeviceQueueCreateInfo[]>(properties_a.queue_families.size());
    const std::unique_ptr<float*[]> vk_queue_prioryties = std::make_unique<float*[]>(properties_a.queue_families.size());

    float prio = 0.3f;

    for (std::size_t i = 0; i < properties_a.queue_families.size(); i++)
    {
        auto itr = std::find_if(gpu_properties.queue_families.begin(),
                                gpu_properties.queue_families.end(),
                                [&](const device::GPU::Properties::Queue_family& qf_a) {
                                    return true == bit::flag::is(qf_a.kind, properties_a.queue_families[i].kind) &&
                                           qf_a.count >= properties_a.queue_families[i].count;
                                });
        if (gpu_properties.queue_families.end() != itr)
        {
            VkDeviceQueueCreateInfo vk_device_queue_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                                                  .pNext = nullptr,
                                                                  .flags = 0x0u,
                                                                  .queueFamilyIndex = static_cast<std::uint32_t>(itr->index),
                                                                  .queueCount =
                                                                      static_cast<std::uint32_t>(properties_a.queue_families[i].count),
                                                                  .pQueuePriorities = &prio
            };

            vk_device_queues_create_info[i] = vk_device_queue_create_info;
        }
        else
        {
            return false;
        }
    }

    std::array<const char*, 3u> ext = { "VK_KHR_swapchain", "VK_EXT_pageable_device_local_memory", "VK_EXT_memory_priority" };
    VkDeviceCreateInfo vk_device_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                               .pNext = nullptr,
                                               .flags = 0x0u,
                                               .queueCreateInfoCount = static_cast<std::uint32_t>(properties_a.queue_families.size()),
                                               .pQueueCreateInfos = vk_device_queues_create_info.get(),
                                               .enabledLayerCount = 0u,
                                               .ppEnabledLayerNames = nullptr,
                                               .enabledExtensionCount = static_cast<std::uint32_t>(properties_a.extensions.size()),
                                               .ppEnabledExtensionNames = ext.data() };

    return VK_SUCCESS == vkCreateDevice(*(p_gpu_a), &vk_device_create_info, nullptr, &(this->vk_device));
}
void renderer::Context::release()
{
    vkDestroyDevice(this->vk_device, nullptr);
    this->vk_device = VK_NULL_HANDLE;
}
} // namespace lxf