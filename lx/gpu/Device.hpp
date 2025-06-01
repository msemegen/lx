#pragma once

// lx
#include <lx/Windower.hpp>
#include <lx/common/bit.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/Buffer.hpp>
#include <lx/gpu/CommandPool.hpp>
#include <lx/gpu/Pipeline.hpp>
#include <lx/gpu/Queue.hpp>
#include <lx/gpu/SwapChain.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cassert>

namespace lx::gpu {
class Device : private lx::common::non_copyable
{
public:
    using Feature = devices::GPU::Feature;

    struct Properties
    {
        Feature features;
        std::span<const char* const> extensions;
    };

    [[nodiscard]] bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_device && false == this->queues.is_empty();
    }

    [[nodiscard]] operator VkDevice() const
    {
        return this->vk_device;
    }

    template<typename Type> [[nodiscard]] Type create(typename const Type::Properties& properties) = delete;
    template<typename Type> void destroy(lx::common::out<Type> object_a) = delete;

private:
    struct QueueInfo
    {
        using Kind = devices::GPU::QueueFamily::Kind;

        using enum Kind;

        Kind kind;
        float priority = 0.0f;

        std::uint32_t index;
        std::uint32_t family;

        bool presentation = false;
    };

    Device(const lx::devices::GPU& gpu_a, VkSurfaceKHR vk_surface_a, const Properties& properties_a);
    void destroy()
    {
        if (VK_NULL_HANDLE != this->vk_device)
        {
            vkDestroyDevice(this->vk_device, nullptr);
            this->vk_device = VK_NULL_HANDLE;
        }
    }

    VkDevice vk_device = VK_NULL_HANDLE;
    VkSurfaceKHR vk_surface = VK_NULL_HANDLE;

    VmaAllocator vk_memory_allocator;

    lx::containers::Vector<QueueInfo> queues;

    friend class Context;
};

template<>
inline [[nodiscard]] lx::gpu::Pipeline<lx::gpu::pipeline::graphics> Device::create<lx::gpu::Pipeline<lx::gpu::pipeline::graphics>>(
    const lx::gpu::Pipeline<lx::gpu::pipeline::graphics>::Properties& properties_a)
{
    return { this->vk_device, properties_a };
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::Pipeline<lx::gpu::pipeline::graphics>> object_a)
{
    object_a->destroy(this->vk_device);
}

template<> inline [[nodiscard]] lx::gpu::Buffer Device::create<lx::gpu::Buffer>(const lx::gpu::Buffer::Properties& properties_a)
{
    return {};
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::Buffer> object_a) {}

template<> inline [[nodiscard]] lx::gpu::SwapChain Device::create<lx::gpu::SwapChain>(const lx::gpu::SwapChain::Properties& properties_a)
{
    return { this->vk_device, this->vk_surface, properties_a };
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::SwapChain> object_a)
{
    object_a->destroy(this->vk_device);
}

template<> inline [[nodiscard]] lx::gpu::Queue Device::create<lx::gpu::Queue>(const lx::gpu::Queue::Properties& properties_a)
{
    for (std::size_t i = 0; i < this->queues.get_length(); i++)
    {
        if (this->queues[i].kind == properties_a.kind && properties_a.presentation == this->queues[i].presentation)
        {
            VkQueue vk_queue;
            const QueueInfo queue_info = this->queues[i];

            this->queues.erase(i);

            vkGetDeviceQueue(this->vk_device, queue_info.family, queue_info.index, &vk_queue);
            return { vk_queue, queue_info.index, queue_info.family, queue_info.kind };
        }
    }

    for (std::size_t i = 0; i < this->queues.get_length(); i++)
    {
        if (true == common::bit::flag::is(this->queues[i].kind, properties_a.kind) &&
            properties_a.presentation == this->queues[i].presentation)
        {
            VkQueue vk_queue;
            const QueueInfo queue_info = this->queues[i];

            this->queues.erase(i);

            vkGetDeviceQueue(this->vk_device, queue_info.family, queue_info.index, &vk_queue);
            return { vk_queue, queue_info.index, queue_info.family, queue_info.kind };
        }
    }

    return { VK_NULL_HANDLE, std::numeric_limits<std::uint32_t>::max(), std::numeric_limits<std::uint32_t>::max(), {} };
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::Queue> object_a) {}
} // namespace lx::gpu