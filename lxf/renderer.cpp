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
#include <lxf/loader/vulkan.hpp>

extern VkInstance vk_instance;

namespace lxf {
using namespace lxf::common;

bool renderer::Context::create(const device::GPU* p_gpu_a, HWND window_handle_a, VkSurfaceKHR vk_surface_a, const Properties& properties_a)
{
    assert(false == renderer::context.is_created());

    const device::GPU::Properties gpu_properties = p_gpu_a->get_properties();
    const float prio_step = 1.0f / static_cast<float>(gpu_properties.limits.discrete_queue_priorities);

    std::vector<VkDeviceQueueCreateInfo> vk_device_queues_create_info;
    vk_device_queues_create_info.reserve(properties_a.queue_families.size());

    const std::unique_ptr<std::vector<float>[]> vk_queue_prioryties =
        std::make_unique<std::vector<float>[]>(properties_a.queue_families.size());

    for (std::size_t qf_property_index = 0; qf_property_index < properties_a.queue_families.size(); qf_property_index++)
    {
        assert(properties_a.queue_families[qf_property_index].count == properties_a.queue_families[qf_property_index].priorities.size());

        auto itr = std::find_if(gpu_properties.queue_families.begin(),
                                gpu_properties.queue_families.end(),
                                [&](const device::GPU::Properties::Queue_family& qf_a) {
                                    return true == bit::flag::is(
                                                       static_cast<std::uint32_t>(qf_a.kind),
                                                       static_cast<std::uint32_t>(properties_a.queue_families[qf_property_index].kind)) &&
                                           qf_a.count >= properties_a.queue_families[qf_property_index].count;
                                });
        if (gpu_properties.queue_families.end() != itr)
        {
            vk_queue_prioryties[qf_property_index].reserve(properties_a.queue_families[qf_property_index].priorities.size());

            for (std::uint16_t qf_priority : properties_a.queue_families[qf_property_index].priorities)
            {
                const float vk_queue_priority = prio_step * qf_priority;

                if (vk_queue_priority <= 1.0f)
                {
                    vk_queue_prioryties[qf_property_index].push_back(vk_queue_priority);
                }
                else
                {
                    return false;
                }
            }

            VkDeviceQueueCreateInfo vk_device_queue_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                                                  .pNext = nullptr,
                                                                  .flags = 0x0u,
                                                                  .queueFamilyIndex = static_cast<std::uint32_t>(itr->index),
                                                                  .queueCount = static_cast<std::uint32_t>(
                                                                      vk_queue_prioryties[qf_property_index].size()),
                                                                  .pQueuePriorities = vk_queue_prioryties[qf_property_index].data() };

            vk_device_queues_create_info.push_back(vk_device_queue_create_info);
        }
        else
        {
            return false;
        }
    }

    VkDeviceCreateInfo vk_device_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                               .pNext = nullptr,
                                               .flags = 0x0u,
                                               .queueCreateInfoCount = static_cast<std::uint32_t>(vk_device_queues_create_info.size()),
                                               .pQueueCreateInfos = vk_device_queues_create_info.data(),
                                               .enabledLayerCount = 0u,
                                               .ppEnabledLayerNames = nullptr,
                                               .enabledExtensionCount = static_cast<std::uint32_t>(properties_a.extensions.size()),
                                               .ppEnabledExtensionNames = properties_a.extensions.data() };

    bool ret = VK_SUCCESS == vkCreateDevice(*(p_gpu_a), &vk_device_create_info, nullptr, &(this->vk_device));

    if (true == ret)
    {
        for (const VkDeviceQueueCreateInfo& vk_queue_descriptor : vk_device_queues_create_info)
        {
            for (std::size_t queue_index = 0u; queue_index < vk_queue_descriptor.queueCount; queue_index++)
            {
                VkQueue vk_queue;
                vkGetDeviceQueue(this->vk_device, vk_queue_descriptor.queueFamilyIndex, static_cast<std::uint32_t>(queue_index), &vk_queue);
                this->vk_queues.push_back(vk_queue);
            }
        }

        this->vk_queues.shrink_to_fit();

        RECT wnd_rect {};
        GetClientRect(window_handle_a, &wnd_rect);
        VkSwapchainCreateInfoKHR vk_swap_chain_create_info {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0x0u,
            .surface = vk_surface_a,
            .minImageCount = static_cast<std::uint32_t>(properties_a.swap_chain.images_count),
            .imageFormat = static_cast<VkFormat>(properties_a.swap_chain.pixel),
            .imageColorSpace = static_cast<VkColorSpaceKHR>(properties_a.swap_chain.color_space),
            .imageExtent = { .width = static_cast<std::uint32_t>(wnd_rect.right - wnd_rect.left),
                             .height = static_cast<std::uint32_t>(wnd_rect.bottom - wnd_rect.top) },
            .imageArrayLayers = 1u,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0u,
            .pQueueFamilyIndices = nullptr,
            .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = static_cast<VkPresentModeKHR>(properties_a.swap_chain.mode),
            .clipped = VK_TRUE,
            .oldSwapchain = VK_NULL_HANDLE
        };
        vkCreateSwapchainKHR(this->vk_device, &vk_swap_chain_create_info, nullptr, &(this->vk_swap_chain));
    }

    return ret;
}

void renderer::Context::release()
{
    assert(true == this->is_created());

    vkDestroySwapchainKHR(this->vk_device, this->vk_swap_chain, nullptr);
    vkDestroyDevice(this->vk_device, nullptr);

    this->vk_device = VK_NULL_HANDLE;
}
} // namespace lxf