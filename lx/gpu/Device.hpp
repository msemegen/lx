#pragma once

// lx
#include <lx/Windower.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/Buffer.hpp>
#include <lx/gpu/Queue.hpp>
#include <lx/gpu/SwapChain.hpp>
#include <lx/gpu/loader/vulkan.hpp>
#include <lx/gpu/pipelines/Graphics.hpp>

// std
#include <cassert>

namespace lx::gpu {
class Device : private lx::common::non_copyable
{
public:
    using Feature = devices::GPU::Feature;

    struct QueueFamily
    {
        using Kind = devices::GPU::QueueFamily::Kind;

        using enum Kind;

        Kind kind;
        std::size_t members = 0;

        lx::containers::Vector<float> priorities;
        bool presentation = false;
    };

    struct Properties
    {
        Feature features;
        std::span<const QueueFamily> queue_families;
        std::span<const char* const> extensions;
    };

    [[nodiscard]] bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_device && false == this->queue_families.is_empty();
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

        std::size_t queue_family;
        std::size_t index;

        bool persentation = false;
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

    lx::containers::Vector<QueueInfo> queue_families;

    friend class Context;
};

template<> inline [[nodiscard]] lx::gpu::pipelines::Graphics
Device::create<lx::gpu::pipelines::Graphics>(const lx::gpu::pipelines::Graphics::Properties& properties_a)
{
    return { this->vk_device, properties_a };
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::pipelines::Graphics> object_a)
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
    // look for first queue with the same proprties in queue_families
    // decrease members, and pop_front from priorities
    // what for queue with multiple types? 
    return {};
}
} // namespace lx::gpu