// this
#include <lx/gpu/CommandList.hpp>

// lx
#include <lx/common/Out.hpp>
#include <lx/gpu/Viewport.hpp>
#include <lx/utils/logger.hpp>

// std
#include <cassert>

namespace {
using namespace lx::common;
using namespace lx::gpu;

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
    VkCommandBufferBeginInfo vk_command_buffer_begin_info { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                                                            .pNext = nullptr,
                                                            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
                                                            .pInheritanceInfo = nullptr };
    return VK_SUCCESS == vkBeginCommandBuffer(vk_command_buffer_a, &vk_command_buffer_begin_info);
}
bool CommandList_stop(VkCommandBuffer vk_command_buffer_a)
{
    return VK_SUCCESS == vkEndCommandBuffer(vk_command_buffer_a);
}

bool CommandList_set_viewport(VkCommandBuffer vk_command_buffer_a, const Viewport& viewport_a)
{
    VkViewport vp { .x = static_cast<float>(viewport_a.position.x),
                    .y = static_cast<float>(viewport_a.position.y),
                    .width = static_cast<float>(viewport_a.size.w),
                    .height = static_cast<float>(viewport_a.size.h),
                    .minDepth = viewport_a.min_depth,
                    .maxDepth = viewport_a.max_depth };
}
} // namespace

namespace lx::gpu {
using namespace lx::common;

CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::CommandList(VkDevice vk_device_a, VkCommandPool vk_command_pool_a)
    : vk_command_buffer(CommandList_create(vk_device_a, vk_command_pool_a))
    , vk_command_pool(vk_command_pool_a)
    , state(State::initial)
{
}

void CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::destroy(VkDevice vk_device_a)
{
    CommandList_destroy(vk_device_a, this->vk_command_pool, out(this->vk_command_buffer));
}

bool CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::start()
{
    assert(State::initial == this->state || State::invalid == this->state);

    if (true == CommandList_start(this->vk_command_buffer))
    {
        this->state = State::recording;

        return true;
    }

    return false;
}
bool CommandList<command_list::Kind::graphics | command_list::Kind::transfer>::stop()
{
    assert(State::recording == this->state);

    if (true == CommandList_stop(this->vk_command_buffer))
    {
        this->state = State::executable;

        return true;
    }

    return false;
}

template<> bool CommandList<command_list::graphics | command_list::transfer>::set<Viewport>(const Viewport& viewport_a)
{
    return false;
}

} // namespace lx::gpu