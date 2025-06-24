#pragma once

// lx
#include <lx/common/non_copyable.hpp>
#include <lx/gpu/Queue.hpp>

// externals
#include <lxl/lxl.h>

// std
#include <cstdint>

namespace lx::gpu {

class CommandPool : public lx::common::non_copyable
{
public:
    void reset()
    {
        vkResetCommandPool(this->vk_device, this->vk_command_pool, 0x0u);
    }

    bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_command_pool;
    }

    operator VkCommandPool() const
    {
        return this->vk_command_pool;
    }

private:
    CommandPool(VkDevice vk_device_a, std::uint32_t queue_family_index_a, lx::gpu::Queue::Kind queue_kind_a);
    void destroy();

    VkCommandPool vk_command_pool = VK_NULL_HANDLE;
    VkDevice vk_device = VK_NULL_HANDLE;

    lx::gpu::Queue::Kind queue_kind;

    friend class Device;
};
} // namespace lx::gpu