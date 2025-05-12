#pragma once

// lx
#include <lx/common/non_copyable.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cstdint>

namespace lx::gpu {
class Queue : private lx::common::non_copyable
{
public:
    struct Properties
    {
        using Kind = devices::GPU::QueueFamily::Kind;

        using enum Kind;

        Kind kind;
        bool presentation = false;
    };

private:
    VkQueue vk_queue;
    std::uint32_t family_index;
};
} // namespace lx::gpu