#pragma once

/*
 *   Name: renderer.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cassert>
#include <cstdint>
#include <utility>

// platform
#include <vulkan/vulkan.hpp>

// lxf
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/device.hpp>

namespace lxf {
struct renderer : private common::non_constructible
{
    class Context : private common::non_copyable
    {
    public:
        struct Properties
        {
            struct Queue_family
            {
                using Kind = device::GPU::Properties::Queue_family::Kind;

                using enum Kind;

                Kind kind;
                std::size_t count;

                std::span<const std::uint16_t> priorities;
            };

            using Feature = device::GPU::Properties::Feature;

            Feature features;
            std::span<const Queue_family> queue_families;
            std::span<const char*> extensions;
        };

        [[nodiscard]] bool is_created() const
        {
            return VK_NULL_HANDLE != this->vk_device;
        }

    private:
        Context()
            : vk_device(VK_NULL_HANDLE)
        {
        }

        bool create(const device::GPU* p_gpu_a, const Properties& properties_a);
        void release();

        VkDevice vk_device;

        std::vector<VkQueue> vk_queues;

        friend struct renderer;
    };

    template<typename Type_t>
    static [[nodiscard]] Context* create(const device::GPU* p_gpu_a, const Context::Properties& properties_a) = delete;
    template<typename Type_t> static void destroy(Type_t** p_object_a) = delete;

private:
    inline static Context context;
};

template<> [[nodiscard]] inline renderer::Context* renderer::create<renderer::Context>(const device::GPU* p_gpu_a,
                                                                                       const Context::Properties& properties_a)
{
    assert(false == context.is_created());

    if (true == context.create(p_gpu_a, properties_a))
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