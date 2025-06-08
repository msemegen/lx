// this
#include <lx/gpu/Device.hpp>

// lx
#include <lx/common/bit.hpp>
#include <lx/gpu/loader/vulkan.hpp>
#include <lx/utils/logger.hpp>

// std
#include <cassert>

extern VkInstance vk_instance;

namespace lx::gpu {
using namespace lx::common;
using namespace lx::containers;
using namespace lx::devices;
using namespace lx::utils;

Device::Device(const GPU& gpu_a, VkSurfaceKHR vk_surface_a, const Properties& properties_a)
{
    Vector<VkDeviceQueueCreateInfo> vk_device_queues_create_info(gpu_a.queue_families.get_length());
    Vector<Vector<float>> priorities(gpu_a.queue_families.get_length());

    for (const auto& queue_family : gpu_a.queue_families)
    {
        priorities.push_back(Vector<float>(queue_family.members, 1.0f));
        VkDeviceQueueCreateInfo vk_device_queue_create_info { .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                                              .pNext = nullptr,
                                                              .flags = 0x0u,
                                                              .queueFamilyIndex = static_cast<std::uint32_t>(queue_family.index),
                                                              .queueCount = static_cast<std::uint32_t>(queue_family.members),
                                                              .pQueuePriorities = priorities.get_back().get_buffer() };

        vk_device_queues_create_info.push_back(vk_device_queue_create_info);

        for (std::size_t i = 0u; i < queue_family.members; i++)
        {
            VkBool32 has_presentation_support = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(
                gpu_a, static_cast<std::uint32_t>(queue_family.index), vk_surface_a, &has_presentation_support);

            this->queues.emplace_back(queue_family.kind,
                                      priorities.get_back()[i],
                                      static_cast<std::uint32_t>(queue_family.index),
                                      static_cast<std::uint32_t>(i),
                                      1u == has_presentation_support);
        }
    }

    if (false == vk_device_queues_create_info.is_empty())
    {
        vk_device_queues_create_info.shrink_to_fit();
        this->queues.shrink_to_fit();

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

        bool success = VK_SUCCESS == vkCreateDevice(vk_instance, gpu_a, &vk_device_create_info, nullptr, &(this->vk_device));

        if (true == success)
        {
            const VmaVulkanFunctions vma_functions { .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
                                                     .vkGetDeviceProcAddr = vkGetDeviceProcAddr };
            VmaAllocatorCreateInfo vma_allocator_create_info { .physicalDevice = gpu_a,
                                                               .device = this->vk_device,
                                                               .preferredLargeHeapBlockSize = 0u,
                                                               .pAllocationCallbacks = nullptr,
                                                               .pDeviceMemoryCallbacks = nullptr,
                                                               .pHeapSizeLimit = nullptr,
                                                               .pVulkanFunctions = &vma_functions,
                                                               .instance = vk_instance };

            success = VK_SUCCESS == vmaCreateAllocator(&vma_allocator_create_info, &this->vk_memory_allocator);

            if (true == success)
            {
                this->vk_surface = vk_surface_a;
            }
        }
    }
    else
    {
        logger::write_line(logger::err, std::source_location::current(), "No compatible queue family found!");
    }
}
} // namespace lx::gpu