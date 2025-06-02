// this
#include <lx/gpu/CommandPool.hpp>

namespace lx::gpu {
CommandPool::CommandPool(VkDevice vk_device_a, std::uint32_t queue_family_index_a)
{
    const VkCommandPoolCreateInfo vk_command_pool_create_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queue_family_index_a,
    };

    vkCreateCommandPool(vk_device_a, &vk_command_pool_create_info, nullptr, &(this->vk_command_pool));
}
} // namespace lx::gpu