#pragma once

// lx
#include <lx/common/non_constructible.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cstdint>

namespace lx::gpu {
struct command_list : private lx::common::non_constructible
{
    enum class Kind : std::uint32_t
    {
        graphics = VK_QUEUE_GRAPHICS_BIT,
        compute = VK_QUEUE_COMPUTE_BIT,
        transfer = VK_QUEUE_TRANSFER_BIT
    };

    using enum Kind;
};

constexpr command_list::Kind operator|(command_list::Kind left_a, command_list::Kind right_a)
{
    return static_cast<command_list::Kind>(static_cast<std::uint32_t>(left_a) | static_cast<std::uint32_t>(right_a));
}
constexpr command_list::Kind operator&(command_list::Kind left_a, command_list::Kind right_a)
{
    return static_cast<command_list::Kind>(static_cast<std::uint32_t>(left_a) & static_cast<std::uint32_t>(right_a));
}

template<command_list::Kind kind> class CommandList : private lx::common::non_constructible
{
};

template<> class CommandList<command_list::Kind::graphics | command_list::Kind::transfer> : private lx::common::non_copyable
{
public:
    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a)
    {
        // TODO: add command list construction
    }

    CommandList(VkCommandBuffer vk_command_buffer_a)
        : vk_command_buffer(vk_command_buffer_a)
    {
    }

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;

    friend class Device;
};

template<> class CommandList<command_list::Kind::graphics> : private lx::common::non_copyable
{
public:
    CommandList(const CommandList<command_list::Kind::graphics | command_list::Kind::transfer>& other_a)
        : vk_command_buffer(other_a)
    {
    }

    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a)
    {
        // TODO: add command list construction
    }

    CommandList(VkCommandBuffer vk_command_buffer_a)
        : vk_command_buffer(vk_command_buffer_a)
    {
    }

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;

    friend class CommandList<command_list::Kind::graphics | command_list::Kind::transfer>;
    friend class Device;
};

template<> class CommandList<command_list::Kind::transfer> : private lx::common::non_copyable
{
public:
    CommandList(const CommandList<command_list::Kind::graphics | command_list::Kind::transfer>& other_a)
        : vk_command_buffer(other_a)
    {
    }
    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a)
    {
        // TODO: add command list construction
    }

    CommandList(VkCommandBuffer vk_command_buffer_a)
        : vk_command_buffer(vk_command_buffer_a)
    {
    }

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;

    friend CommandList<command_list::Kind::graphics | command_list::Kind::transfer>;
    friend class Device;
};

template<> class CommandList<command_list::Kind::compute> : private lx::common::non_copyable
{
public:
    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a)
    {
        // TODO: add command list construction
    }

    CommandList(VkCommandBuffer vk_command_buffer_a)
        : vk_command_buffer(vk_command_buffer_a)
    {
    }

    VkCommandBuffer vk_command_buffer;

    friend class Device;
};

} // namespace lx::gpu