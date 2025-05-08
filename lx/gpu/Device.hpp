#pragma once

// lx
#include <lx/Windower.hpp>
#include <lx/common/non_constructible.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/Buffer.hpp>
#include <lx/gpu/loader/vulkan.hpp>
#include <lx/gpu/pipelines/Graphics.hpp>

// std
#include <cassert>

namespace lx::gpu {
class Device : private lx::common::non_copyable
{
public:
    using Feature = devices::GPU::Feature;

    struct s : private lx::common::non_constructible
    {
        static constexpr std::size_t graphic_pipelines_buffer_capacity = 128u;
    };

    struct QueueFamily
    {
        using Kind = devices::GPU::QueueFamily::Kind;

        using enum Kind;

        Kind kind;
        std::size_t count = 0u;

        lx::containers::Vector<float> priorities;
        bool presentation = false;
    };
    struct SwapChain
    {
        using Format = loader::vulkan::Format;

        enum class ColorSpace : std::uint64_t
        {
            srgb_nonlinear_khr = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            display_p3_nonlinear_ext = VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT,
            extended_srgb_linear_ext = VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
            display_p3_linear_ext = VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
            dci_p3_nonlinear_ext = VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,
            bt709_linear_ext = VK_COLOR_SPACE_BT709_LINEAR_EXT,
            bt709_nonlinear_ext = VK_COLOR_SPACE_BT709_NONLINEAR_EXT,
            bt2020_linear_ext = VK_COLOR_SPACE_BT2020_LINEAR_EXT,
            hdr10_st2084_ext = VK_COLOR_SPACE_HDR10_ST2084_EXT,
            dolbyvision_ext = VK_COLOR_SPACE_DOLBYVISION_EXT,
            hdr10_hlg_ext = VK_COLOR_SPACE_HDR10_HLG_EXT,
            adobergb_linear_ext = VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,
            adobergb_nonlinear_ext = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT,
            pass_through_ext = VK_COLOR_SPACE_PASS_THROUGH_EXT,
            extended_srgb_nonlinear_ext = VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT,
            display_native_amd = VK_COLOR_SPACE_DISPLAY_NATIVE_AMD,
            space_dci_p3_linear_ext = VK_COLOR_SPACE_DCI_P3_LINEAR_EXT,
        };
        enum class Mode : std::uint64_t
        {
            immediate = VK_PRESENT_MODE_IMMEDIATE_KHR,
            mailbox = VK_PRESENT_MODE_MAILBOX_KHR,
            fifo = VK_PRESENT_MODE_FIFO_KHR,
            fifo_relaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
            shared_demand_refresh = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR,
            shared_continuous_refresh = VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR,
        };

        Format format;
        ColorSpace color_space;
        Mode mode;

        std::size_t images_count;
    };

    struct Properties
    {
        Feature features;
        std::span<const QueueFamily> queue_families;
        std::span<const char* const> extensions;
        SwapChain swap_chain;
    };

    [[nodiscard]] bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_device && VK_NULL_HANDLE != this->vk_swap_chain && false == this->vk_queues.is_empty() &&
               false == vk_swap_chain_images.is_empty();
    }

    [[nodiscard]] operator VkDevice() const
    {
        return this->vk_device;
    }
    [[nodiscard]] operator VkSwapchainKHR() const
    {
        return this->vk_swap_chain;
    }

    template<typename Type> [[nodiscard]] Type* create(typename const Type::Properties& properties) = delete;
    template<typename Type> void destroy(lx::common::out<Type*> device_a) = delete;

private:
    void create(const lx::devices::GPU& gpu_a,
                VkSurfaceKHR vk_surface_a,
                const VkExtent2D& swap_buffer_extent_a,
                const Properties& properties_a);
    void destroy()
    {
        if (VK_NULL_HANDLE != this->vk_swap_chain)
        {
            vkDestroySwapchainKHR(this->vk_device, this->vk_swap_chain, nullptr);
            this->vk_swap_chain = VK_NULL_HANDLE;
        }

        if (VK_NULL_HANDLE != this->vk_device)
        {
            vkDestroyDevice(this->vk_device, nullptr);
            this->vk_device = VK_NULL_HANDLE;
        }
    }

    VkDevice vk_device = VK_NULL_HANDLE;
    VkSwapchainKHR vk_swap_chain = VK_NULL_HANDLE;

    lx::containers::Vector<VkQueue> vk_queues;
    lx::containers::Vector<VkImage> vk_swap_chain_images;
    lx::containers::Vector<VkImageView> vk_swap_chain_image_views;

    lx::containers::Vector<lx::gpu::pipelines::Graphics, s::graphic_pipelines_buffer_capacity> graphics_pipelines;

    friend class Context;
};

template<> inline [[nodiscard]] lx::gpu::pipelines::Graphics*
Device::create<lx::gpu::pipelines::Graphics>(const lx::gpu::pipelines::Graphics::Properties& properties_a)
{
    lx::gpu::pipelines::Graphics pipeline;
    pipeline.create(properties_a);

    if (true == pipeline.is_created())
    {
        this->graphics_pipelines.emplace_back(std::move(pipeline));
        return &(this->graphics_pipelines.get_back());
    }

    return nullptr;
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::pipelines::Graphics*> object_a) {}

template<> inline [[nodiscard]] lx::gpu::Buffer* Device::create<lx::gpu::Buffer>(const lx::gpu::Buffer::Properties& properties_a)
{
    return nullptr;
}
template<> inline void Device::destroy(lx::common::out<lx::gpu::Buffer*> object_a) {}
} // namespace lx::gpu