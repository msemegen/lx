// this
#include <lx/gpu/CommandList.hpp>

// lx
#include <lx/common/Out.hpp>
#include <lx/utils/logger.hpp>

// std
#include <cassert>

namespace {
using namespace lx::common;

VkCommandBuffer CommandList_create(VkDevice vk_device_a, VkCommandPool vk_command_pool_a)
{
    VkCommandBufferAllocateInfo vk_allocate_command_buffer_info { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                                                  .pNext = nullptr,
                                                                  .commandPool = vk_command_pool_a,
                                                                  .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                                                  .commandBufferCount = 1u };

    VkCommandBuffer vk_command_buffer = VK_NULL_HANDLE;
    auto res = vkAllocateCommandBuffers(vk_device_a, &vk_allocate_command_buffer_info, &(vk_command_buffer));

    if (VK_SUCCESS != res)
    {
        log_err("vkAllocateCommandBuffers failure: {}", static_cast<std::underlying_type_t<decltype(res)>>(res));
    }

    return vk_command_buffer;
}
void CommandList_destroy(VkDevice vk_device_a, VkCommandPool vk_command_pool_a, out<VkCommandBuffer> vk_command_buffer_a)
{
    vkFreeCommandBuffers(vk_device_a, vk_command_pool_a, 1u, &(*vk_command_buffer_a));
    (*vk_command_buffer_a) = VK_NULL_HANDLE;
}

bool CommandList_start(VkCommandBuffer vk_command_buffer_a)
{
    VkCommandBufferBeginInfo vk_command_buffer_begin_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .pNext = nullptr, .flags = 0u, .pInheritanceInfo = nullptr
    };
    return VK_SUCCESS == vkBeginCommandBuffer(vk_command_buffer_a, &vk_command_buffer_begin_info);
}
bool CommandList_stop(VkCommandBuffer vk_command_buffer_a)
{
    return VK_SUCCESS == vkEndCommandBuffer(vk_command_buffer_a);
}
} // namespace

namespace lx::gpu {
using namespace lx::common;

CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::CommandList(VkDevice vk_device_a, VkCommandPool vk_command_pool_a)
    : vk_device(vk_device_a)
    , vk_command_pool(vk_command_pool_a)
    , vk_command_buffer(CommandList_create(this->vk_device, this->vk_command_pool))
{
}

void CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::destroy()
{
    CommandList_destroy(this->vk_device, this->vk_command_pool, out(this->vk_command_buffer));
}

bool CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::start()
{
    assert(false == this->is_started());

    this->started = CommandList_start(this->vk_command_buffer);
    return this->started;
}
bool CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::stop()
{
    assert(true == this->is_started());

    if (true == CommandList_stop(this->vk_command_buffer))
    {
        this->started = false;
    }

    return false == this->started;
}
} // namespace lx::gpu