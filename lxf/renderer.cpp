// this
#include <lxf/renderer.hpp>

// platform
#include <vulkan/vulkan.h>

namespace lxf {
renderer::Context::Context(const device::GPU& gpu_a, common::Uint32 queue_family_index, device::GPU::Feature features_a)
{
    VkDeviceCreateInfo vk_device_create_info;

    // vkCreateDevice();
}
} // namespace lxf