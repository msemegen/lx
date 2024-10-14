#pragma once

/*
 *   Name: renderer.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>

// platform
#include <vulkan/vulkan.hpp>

// lxf
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/common/scalar.hpp>
#include <lxf/device.hpp>

namespace lxf {
struct renderer : private common::non_constructible
{
    class Context : private common::non_copyable
    {
    public:
        [[nodiscard]] bool is_created() const
        {
            return VK_NULL_HANDLE != this->vk_device;
        }

    private:
        Context()
            : vk_device(VK_NULL_HANDLE)
        {
        }

        bool create(const device::GPU& gpu_a, std::span<const device::GPU::Queue> queues_a, device::GPU::Feature features_a);
        void release();

        VkDevice vk_device;

        friend struct renderer;
    };

    template<typename Type_t> static [[nodiscard]] Context*
    create(const device::GPU& gpu_a, std::span<const device::GPU::Queue> queues_a, device::GPU::Feature features_a) = delete;
    template<typename Type_t> static void destroy(Type_t** p_object_a) = delete;

private:
    inline static Context context;
};

template<> [[nodiscard]] inline renderer::Context*
renderer::create<renderer::Context>(const device::GPU& gpu_a, std::span<const device::GPU::Queue> queues_a, device::GPU::Feature features_a)
{
    assert(false == context.is_created());

    if (true == context.create(gpu_a, queues_a, features_a))
    {
        return &(context);
    }

    return nullptr;
}
template<> inline void renderer::destroy<renderer::Context>(renderer::Context** p_context_a)
{
    assert(true == context.is_created());

    (*p_context_a)->release();
    *p_context_a = nullptr;
}
} // namespace lxf