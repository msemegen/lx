#pragma once

// lx
#include <lx/common/non_copyable.hpp>
#include <lx/gpu/CommandList.hpp>

// externals
#include <lxl/lxl.h>

namespace lx::gpu {
class Buffer : lx::common::non_copyable
{
public:
    struct Properties
    {
    };

    Buffer() {}

private:
    Buffer(VkDevice vk_device_a, CommandList<command_list::transfer>* p_command_list_a, const Properties& properties_a) {}

    friend class Device;
};
} // namespace lx::gpu