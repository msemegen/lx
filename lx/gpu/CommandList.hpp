#pragma once

// lx
#include <lx/common/non_constructible.hpp>
#include <lx/common/non_copyable.hpp>

// externals
#include <lxl/lxl.h>

// std
#include <cstdint>

namespace lx::gpu {
struct Viewport;

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

template<command_list::Kind kind> class CommandList;
template<> class CommandList<command_list::Kind::graphics>;
template<> class CommandList<command_list::Kind::transfer>;
template<> class CommandList<command_list::Kind::graphics | command_list::Kind::transfer>;

template<command_list::Kind kind> class CommandList : private lx::common::non_constructible
{
};

template<> class CommandList<command_list::graphics | command_list::transfer> : private lx::common::non_copyable
{
public:
    enum State : std::uint32_t
    {
        initial,
        recording,
        executable,
        pending,
        invalid
    };

    bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_command_buffer;
    }

    State get_state() const
    {
        return this->state;
    }

    bool start();
    bool stop();

    template<typename Type> bool set(const Type& type_a) = delete;

    // graphics pipeline functions:
    void bind();
    void draw();

    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a, VkCommandPool vk_command_pool_a);
    void destroy(VkDevice vk_device_a);

    State state;

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;
    VkCommandPool vk_command_pool = VK_NULL_HANDLE;

    friend class CommandList<command_list::graphics>;
    friend class CommandList<command_list::transfer>;

    friend class Device;
};

template<> class CommandList<command_list::graphics> : private lx::common::non_copyable
{
public:
    CommandList(const CommandList<command_list::graphics | command_list::transfer>& other_a)
        : vk_command_buffer(other_a.vk_command_buffer)
        , vk_command_pool(other_a.vk_command_pool)
    {
    }

    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

    // graphics pipeline functions:
    void bind_pipeline();
    void set_view_port();
    void draw();

private:
    CommandList(VkDevice vk_device_a, VkCommandPool vk_command_pool_a);
    void destroy(VkDevice vk_device_a);

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;
    VkCommandPool vk_command_pool = VK_NULL_HANDLE;

    friend class CommandList<command_list::graphics | command_list::transfer>;
    friend class Device;
};

template<> class CommandList<command_list::transfer> : private lx::common::non_copyable
{
public:
    CommandList(const CommandList<command_list::graphics | command_list::transfer>& other_a)
        : vk_command_buffer(other_a.vk_command_buffer)
        , vk_command_pool(other_a.vk_command_pool)
    {
    }
    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a, VkCommandPool vk_command_pool_a);
    void destroy(VkDevice vk_device_a);

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;
    VkCommandPool vk_command_pool = VK_NULL_HANDLE;

    friend class CommandList<command_list::graphics | command_list::transfer>;
    friend class Device;
};

template<> class CommandList<command_list::compute> : private lx::common::non_copyable
{
public:
    operator VkCommandBuffer() const
    {
        return this->vk_command_buffer;
    }

private:
    CommandList(VkDevice vk_device_a, VkCommandPool vk_command_pool_a);
    void destroy(VkDevice vk_device_a);

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;
    VkCommandPool vk_command_pool = VK_NULL_HANDLE;

    friend class Device;
};

template<> bool CommandList<command_list::graphics | command_list::transfer>::set<Viewport>(const Viewport& viewport_a);
} // namespace lx::gpu