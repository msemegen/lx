/*
 *   Name: renderer.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/renderer.hpp>

// std
#include <memory>

extern VkInstance vk_instance;

namespace lxf {
bool renderer::Context::create(const device::GPU& gpu_a, std::span<const device::GPU::Queue> queues_a, device::GPU::Feature)
{
    const std::unique_ptr<VkDeviceQueueCreateInfo[]> vk_device_queues_create_info =
        std::make_unique<VkDeviceQueueCreateInfo[]>(queues_a.size());

    for (std::size_t i = 0; i < queues_a.size(); i++)
    {
        VkDeviceQueueCreateInfo vk_device_queue_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .pNext = nullptr };
        vk_device_queues_create_info[i] = vk_device_queue_create_info;
    }

    VkDeviceCreateInfo vk_device_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                               .pNext = nullptr,
                                               .flags = 0x0u,
                                               .queueCreateInfoCount = static_cast<std::uint32_t>(queues_a.size()),
                                               .pQueueCreateInfos = vk_device_queues_create_info.get(),
                                               .enabledLayerCount = 0u,
                                               .ppEnabledLayerNames = nullptr };

    return VK_SUCCESS == vkCreateDevice(gpu_a, &vk_device_create_info, nullptr, &(this->vk_device));
}
void renderer::Context::release()
{
    vkDestroyDevice(this->vk_device, nullptr);
    this->vk_device = VK_NULL_HANDLE;
}
} // namespace lxf