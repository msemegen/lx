// this
#include <lx/gpu/Queue.hpp>

namespace lx::gpu {
template<> bool Queue::submit<command_list::graphics | command_list::transfer>(
    const CommandList<command_list::graphics | command_list::transfer>& command_list_a)
{
    VkSubmitInfo vk_submit_info {};
    //vkQueueSubmit(this->vk_queue, 1u, &vk_submit_info, vk_fence);
    return false;
}
} // namespace lx::gpu