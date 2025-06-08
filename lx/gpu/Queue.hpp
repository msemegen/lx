#pragma once

// lx
#include <lx/common/non_copyable.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/CommandList.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cstdint>

namespace lx::gpu {
class Queue : private lx::common::non_copyable
{
public:
    using Kind = devices::GPU::QueueFamily::Kind;
    using enum Kind;

    struct Properties
    {
        Kind kind;
        bool presentation = false;
    };

    template<command_list::Kind kind> bool submit(const CommandList<kind>& command_list_a) = delete;

    bool is_created()
    {
        return VK_NULL_HANDLE != this->vk_queue;
    }

private:
    Queue(VkQueue vk_queue_a, std::uint32_t index_a, std::uint32_t family_a, Kind kind_a)
        : vk_queue(vk_queue_a)
        , index(index_a)
        , family(family_a)
        , kind(kind_a)
    {
    }

    VkQueue vk_queue = VK_NULL_HANDLE;

    std::uint32_t index = 0u;
    std::uint32_t family = 0u;

    Kind kind;

    friend class Device;
};

template<> bool Queue::submit<command_list::graphics | command_list::transfer>(
    const CommandList<command_list::graphics | command_list::transfer>& command_list_a);
} // namespace lx::gpu