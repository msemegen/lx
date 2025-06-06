// this
#include <lx/gpu/Device.hpp>

// lx
#include <lx/common/bit.hpp>
#include <lx/gpu/loader/vulkan.hpp>
#include <lx/utils/logger.hpp>

// std
#include <cassert>

namespace lx::gpu {
using namespace lx::common;
using namespace lx::containers;
using namespace lx::devices;
using namespace lx::utils;

Device::Device(const GPU& gpu_a, VkSurfaceKHR vk_surface_a, const VkExtent2D& swap_buffer_extent_a, const Properties& properties_a)
{
    assert(false == properties_a.queue_families.empty());
    assert(false == gpu_a.queue_families.is_empty());

    Vector<VkDeviceQueueCreateInfo> vk_device_queues_create_info(properties_a.queue_families.size());

    for (std::size_t qf_property_index = 0; qf_property_index < properties_a.queue_families.size(); qf_property_index++)
    {
        assert(properties_a.queue_families[qf_property_index].count ==
               properties_a.queue_families[qf_property_index].priorities.get_length());

        auto itr = std::find_if(begin(gpu_a.queue_families), end(gpu_a.queue_families), [&](const GPU::QueueFamily& gpu_qf_a) {
            VkBool32 has_presentation_support = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(
                gpu_a, static_cast<std::uint32_t>(gpu_qf_a.index), vk_surface_a, &has_presentation_support);

            return true == bit::flag::is(gpu_qf_a.kind, properties_a.queue_families[qf_property_index].kind) &&
                   gpu_qf_a.count >= properties_a.queue_families[qf_property_index].count &&
                   static_cast<VkBool32>(properties_a.queue_families[qf_property_index].presentation) == has_presentation_support;
        });
        if (end(gpu_a.queue_families) != itr)
        {
            VkDeviceQueueCreateInfo vk_device_queue_create_info {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0x0u,
                .queueFamilyIndex = static_cast<std::uint32_t>(itr->index),
                .queueCount = static_cast<std::uint32_t>(properties_a.queue_families[qf_property_index].priorities.get_length()),
                .pQueuePriorities = properties_a.queue_families[qf_property_index].priorities.get_buffer()
            };

            vk_device_queues_create_info.push_back(vk_device_queue_create_info);
        }
    }

    if (false == vk_device_queues_create_info.is_empty())
    {
        Vector<const char*> extensions(properties_a.extensions.size() + 1u);
        extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        extensions.push_back(properties_a.extensions);

        VkDeviceCreateInfo vk_device_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                                   .pNext = nullptr,
                                                   .flags = 0x0u,
                                                   .queueCreateInfoCount =
                                                       static_cast<std::uint32_t>(vk_device_queues_create_info.get_length()),
                                                   .pQueueCreateInfos = vk_device_queues_create_info.get_buffer(),
                                                   .enabledLayerCount = 0u,
                                                   .ppEnabledLayerNames = nullptr,
                                                   .enabledExtensionCount = static_cast<std::uint32_t>(extensions.get_length()),
                                                   .ppEnabledExtensionNames = extensions.get_buffer() };

        bool success = VK_SUCCESS == vkCreateDevice(gpu_a, &vk_device_create_info, nullptr, &(this->vk_device));

        if (true == success)
        {
            for (const VkDeviceQueueCreateInfo& vk_queue_descriptor : vk_device_queues_create_info)
            {
                for (std::size_t queue_index = 0u; queue_index < vk_queue_descriptor.queueCount; queue_index++)
                {
                    VkQueue vk_queue;
                    vkGetDeviceQueue(
                        this->vk_device, vk_queue_descriptor.queueFamilyIndex, static_cast<std::uint32_t>(queue_index), &vk_queue);
                    this->vk_queues.push_back(vk_queue);
                }
            }

            this->vk_queues.shrink_to_fit();

            VkSwapchainCreateInfoKHR vk_swap_chain_create_info { .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                                                 .pNext = nullptr,
                                                                 .flags = 0x0u,
                                                                 .surface = vk_surface_a,
                                                                 .minImageCount =
                                                                     static_cast<std::uint32_t>(properties_a.swap_chain.images_count),
                                                                 .imageFormat = static_cast<VkFormat>(properties_a.swap_chain.format),
                                                                 .imageColorSpace =
                                                                     static_cast<VkColorSpaceKHR>(properties_a.swap_chain.color_space),
                                                                 .imageExtent = swap_buffer_extent_a,
                                                                 .imageArrayLayers = 1u,
                                                                 .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                                                 .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                                                 .queueFamilyIndexCount = 0u,
                                                                 .pQueueFamilyIndices = nullptr,
                                                                 .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                                                 .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                                                 .presentMode = static_cast<VkPresentModeKHR>(properties_a.swap_chain.mode),
                                                                 .clipped = VK_TRUE,
                                                                 .oldSwapchain = VK_NULL_HANDLE };

            success = VK_SUCCESS == vkCreateSwapchainKHR(this->vk_device, &vk_swap_chain_create_info, nullptr, &(this->vk_swap_chain));

            if (true == success)
            {
                std::uint32_t image_count = 0;

                vkGetSwapchainImagesKHR(this->vk_device, this->vk_swap_chain, &image_count, nullptr);
                this->vk_swap_chain_images.reserve(image_count);
                vkGetSwapchainImagesKHR(this->vk_device, this->vk_swap_chain, &image_count, this->vk_swap_chain_images.get_buffer());
            }
        }
    }
    else
    {
        logger::write_line(logger::err, std::source_location::current(), "No compatible queue family found!");
    }
}
} // namespace lx::gpu