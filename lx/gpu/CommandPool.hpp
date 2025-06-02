#pragma once

// lx
#include <lx/common/non_copyable.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cstdint>

namespace lx::gpu {
class Queue;

class CommandPool : public lx::common::non_copyable
{
public:
    bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_command_pool;
    }

    operator VkCommandPool() const
    {
        return this->vk_command_pool;
    }

private:
    CommandPool(VkDevice vk_device_a, std::uint32_t queue_family_index_a);

    VkCommandPool vk_command_pool = VK_NULL_HANDLE;

    friend class Device;
};
} // namespace lx::gpu