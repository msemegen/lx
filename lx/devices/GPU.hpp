#pragma once

/*
 *	Name: GPU.hpp
 *  Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lx
#include <lx/containers/String.hpp>
#include <lx/containers/Vector.hpp>

// externals
#include <lxl/lxl.h>

// std
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace lx::devices {
struct GPU
{
    enum class Kind : std::uint32_t
    {
        software = 0x1u,
        discrete = 0x2u,
        integrated = 0x4u,
        indirect = 0x8u,
        primary = 0x10u
    };
    enum class Feature : std::uint64_t
    {
        none = 0x0u,
        robust_buffer_access = 0x1ull,
        full_draw_index_uint32 = 0x2ull,
        image_cube_array = 0x4ull,
        independent_blend = 0x8ull,
        geometry_shader = 0x10ull,
        tessellation_shader = 0x20ull,
        sample_rate_shading = 0x40ull,
        dual_src_blend = 0x80ull,
        logic_op = 0x100ull,
        multi_draw_indirect = 0x200ull,
        draw_indirect_first_instance = 0x400ull,
        depth_clamp = 0x800ull,
        depth_bias_clamp = 0x1000ull,
        fill_mode_non_nolid = 0x2000ull,
        depth_bounds = 0x4000ull,
        wide_lines = 0x8000ull,
        large_points = 0x10000ull,
        alpha_to_one = 0x20000ull,
        multi_viewport = 0x40000ull,
        sampler_anisotropy = 0x60000ull,
        texture_compression_ETC2 = 0x80000ull,
        texture_compression_ASTC_LDR = 0x100000ull,
        texture_compression_BC = 0x200000ull,
        occlusion_query_precise = 0x400000ull,
        pipeline_statistics_query = 0x800000ull,
        vertex_pipeline_stores_andatomics = 0x1000000ull,
        fragment_stores_and_atomics = 0x2000000ull,
        shader_tessellation_and_geometry_point_size = 0x4000000ull,
        shader_image_gather_extended = 0x6000000ull,
        shader_storage_image_extended_formats = 0x8000000ull,
        shader_storage_image_multisample = 0x10000000ull,
        shader_storage_image_read_without_format = 0x20000000ull,
        shader_storage_image_write_without_format = 0x40000000ull,
        shader_uniform_buffer_array_dynamic_indexing = 0x60000000ull,
        shader_sampled_image_array_dynamic_indexing = 0x80000000ull,
        shader_storage_buffer_array_dynamic_indexing = 0x100000000ull,
        shader_storage_image_array_dynamic_indexing = 0x200000000ull,
        shader_clip_distance = 0x400000000ull,
        shader_cull_distance = 0x600000000ull,
        shader_float64 = 0x800000000ull,
        shader_int64 = 0x1000000000ull,
        shader_int16 = 0x2000000000ull,
        shader_resource_residency = 0x4000000000ull,
        shader_resource_min_lod = 0x6000000000ull,
        sparse_binding = 0x8000000000ull,
        sparse_residency_buffer = 0x10000000000ull,
        sparse_residency_image2d = 0x20000000000ull,
        sparse_residency_image3d = 0x40000000000ull,
        sparse_residency_2_samples = 0x80000000000ull,
        sparse_residency_4_samples = 0x100000000000ull,
        sparse_residency_8_samples = 0x200000000000ull,
        sparse_residency_16_samples = 0x400000000000ull,
        sparse_residency_aliased = 0x800000000000ull,
        variable_multisample_rate = 0x1000000000000ull,
        inherited_queries = 0x2000000000000ull
    };

    struct Limits
    {
        enum class Sample_count : std::uint32_t
        {
            _1 = VK_SAMPLE_COUNT_1_BIT,
            _2 = VK_SAMPLE_COUNT_2_BIT,
            _4 = VK_SAMPLE_COUNT_4_BIT,
            _8 = VK_SAMPLE_COUNT_8_BIT,
            _16 = VK_SAMPLE_COUNT_16_BIT,
            _32 = VK_SAMPLE_COUNT_32_BIT,
            _64 = VK_SAMPLE_COUNT_64_BIT
        };

        std::uint32_t max_image_dimension_1d = 0u;
        std::uint32_t max_image_dimension_2d = 0u;
        std::uint32_t max_image_dimension_3d = 0u;
        std::uint32_t max_image_dimensioncube = 0u;
        std::uint32_t max_image_arraylayers = 0u;
        std::uint32_t max_texel_buffer_elements = 0u;
        std::uint32_t max_uniform_buffer_range = 0u;
        std::uint32_t max_storage_buffer_range = 0u;
        std::uint32_t max_push_constants_size = 0u;
        std::uint32_t max_memory_allocation_count = 0u;
        std::uint32_t max_sampler_allocation_count = 0u;
        std::uint64_t buffer_image_granularity = 0ull;
        std::uint64_t sparse_address_space_size = 0ull;
        std::uint32_t max_bound_descriptor_sets = 0u;
        std::uint32_t max_per_stage_descriptor_samplers = 0u;
        std::uint32_t max_per_stage_descriptor_uniform_buffers = 0u;
        std::uint32_t max_per_stage_descriptor_storage_buffers = 0u;
        std::uint32_t max_per_stage_descriptor_sampled_images = 0u;
        std::uint32_t max_per_stage_descriptor_storage_images = 0u;
        std::uint32_t max_per_stage_descriptor_input_attachments = 0u;
        std::uint32_t max_per_stage_resources = 0u;
        std::uint32_t max_descriptor_set_samplers = 0u;
        std::uint32_t max_descriptor_set_uniform_buffers = 0u;
        std::uint32_t max_descriptor_set_uniform_buffers_dynamic = 0u;
        std::uint32_t max_descriptor_set_storage_buffers = 0u;
        std::uint32_t max_descriptor_set_storage_buffers_dynamic = 0u;
        std::uint32_t max_descriptor_set_sampled_images = 0u;
        std::uint32_t max_descriptor_set_storage_images = 0u;
        std::uint32_t max_descriptor_set_input_attachments = 0u;
        std::uint32_t max_vertex_input_attributes = 0u;
        std::uint32_t max_vertex_input_bindings = 0u;
        std::uint32_t max_vertex_input_attribute_offset = 0u;
        std::uint32_t max_vertex_input_binding_stride = 0u;
        std::uint32_t max_vertex_output_components = 0u;
        std::uint32_t max_tessellation_generationlevel = 0u;
        std::uint32_t max_tessellation_patchsize = 0u;
        std::uint32_t max_tessellation_control_per_vertex_input_components = 0u;
        std::uint32_t max_tessellation_control_per_vertex_output_components = 0u;
        std::uint32_t max_tessellation_control_per_patch_output_components = 0u;
        std::uint32_t max_tessellation_control_total_output_components = 0u;
        std::uint32_t max_tessellation_evaluation_input_components = 0u;
        std::uint32_t max_tessellation_evaluation_output_components = 0u;
        std::uint32_t max_geometry_shader_invocations = 0u;
        std::uint32_t max_geometry_input_components = 0u;
        std::uint32_t max_geometry_output_components = 0u;
        std::uint32_t max_geometry_output_vertices = 0u;
        std::uint32_t max_geometry_total_output_components = 0u;
        std::uint32_t max_fragment_input_components = 0u;
        std::uint32_t max_fragment_output_attachments = 0u;
        std::uint32_t max_fragment_dual_src_attachments = 0u;
        std::uint32_t max_fragment_combined_output_resources = 0u;
        std::uint32_t max_compute_shared_memory_size = 0u;
        std::uint32_t max_compute_work_group_count[3] = { 0u, 0u, 0u };
        std::uint32_t max_compute_work_group_invocations = 0u;
        std::uint32_t max_compute_work_group_size[3] = { 0u, 0u, 0u };
        std::uint32_t sub_pixel_precision_bits = 0u;
        std::uint32_t sub_texel_precision_bits = 0u;
        std::uint32_t mipmap_precision_bits = 0u;
        std::uint32_t max_draw_indexed_index_value = 0u;
        std::uint32_t max_draw_indirect_count = 0u;
        float max_sampler_lod_bias = 0.0f;
        float max_sampler_anisotropy = 0.0f;
        std::uint32_t max_viewports = 0u;
        std::uint32_t max_viewport_dimensions[2] = { 0u, 0u };
        float viewport_bounds_range[2] = { 0.0f, 0.0f };
        std::uint32_t viewport_sub_pixel_bits = 0u;
        std::uint64_t min_memory_map_alignment = 0ull;
        std::uint64_t min_texel_buffer_offset_alignment = 0ull;
        std::uint64_t min_uniform_buffer_offset_alignment = 0ull;
        std::uint64_t min_storage_buffer_offset_alignment = 0ull;
        std::int32_t min_texel_offset = 0;
        std::uint32_t max_texel_offset = 0u;
        std::int32_t min_texel_gather_offset = 0;
        std::uint32_t max_texel_gather_offset = 0u;
        float min_interpolation_offset = 0.0f;
        float max_interpolation_offset = 0.0f;
        std::uint32_t sub_pixel_interpolation_offset_bits = 0u;
        std::uint32_t max_framebuffer_width = 0u;
        std::uint32_t max_framebuffer_height = 0u;
        std::uint32_t max_framebuffer_layers = 0u;
        Sample_count framebuffer_color_sample_counts;
        Sample_count framebuffer_depth_sample_counts;
        Sample_count framebuffer_stencil_sample_counts;
        Sample_count framebuffer_no_attachments_sample_counts;
        std::uint32_t max_color_attachments = 0u;
        Sample_count sampled_image_color_sample_counts;
        Sample_count sampled_image_integer_sample_counts;
        Sample_count sampled_image_depth_sample_counts;
        Sample_count sampled_image_stencil_sample_counts;
        Sample_count storage_image_sample_counts;
        std::uint32_t max_sample_mask_words = 0u;
        bool timestamp_compute_and_graphics = false;
        float timestamp_period = 0.0f;
        std::uint32_t max_clip_distances = 0u;
        std::uint32_t max_cull_distances = 0u;
        std::uint32_t max_combined_clip_and_cull_distances = 0u;
        std::uint32_t discrete_queue_priorities = 0u;
        float point_size_range[2] = { 0.0f, 0.0f };
        float line_width_hrange[2] = { 0.0f, 0.0f };
        float point_size_granularity = 0.0f;
        float line_width_granularity = 0.0f;
        bool strict_lines = false;
        bool standard_sample_locations = false;
        std::uint64_t optimal_buffer_copy_offset_alignment = 0ull;
        std::uint64_t optimal_buffer_copy_row_pitch_alignment = 0ull;
        std::uint64_t non_coherent_atom_size = 0ull;
    };
    struct QueueFamily
    {
        enum class Kind : std::uint32_t
        {
            graphics = VK_QUEUE_GRAPHICS_BIT,
            compute = VK_QUEUE_COMPUTE_BIT,
            transfer = VK_QUEUE_TRANSFER_BIT,
            sparse_binding = VK_QUEUE_SPARSE_BINDING_BIT,
            protected_memory = VK_QUEUE_PROTECTED_BIT,
            video_decode = VK_QUEUE_VIDEO_DECODE_BIT_KHR,
            video_encode = VK_QUEUE_VIDEO_ENCODE_BIT_KHR,
            optical_flow = VK_QUEUE_OPTICAL_FLOW_BIT_NV,
        };

        using enum Kind;

        Kind kind;
        std::size_t members;
        std::size_t index;
    };

    using enum Kind;

    operator VkPhysicalDevice() const
    {
        return this->vk_physical_device;
    }

    const Kind kind = {};
    const Feature features = {};
    const Limits limits = {};

    const lx::containers::String<char> name = {};

    const lx::containers::Vector<QueueFamily> queue_families = {};
    const lx::containers::Vector<std::string_view> extensions = {};

    GPU() = default;
    GPU(const GPU&) = default;

    GPU(VkPhysicalDevice vk_physical_device_a,
        Kind kind_a,
        Feature features_a,
        Limits limits_a,
        std::string_view name_a,
        const lx::containers::Vector<QueueFamily>& queue_families_a,
        const lx::containers::Vector<std::string_view>& extensions_a)
        : kind(kind_a)
        , features(features_a)
        , limits(limits_a)
        , name(name_a)
        , queue_families(queue_families_a)
        , extensions(extensions_a)
        , vk_physical_device(vk_physical_device_a)
    {
    }

private:
    VkPhysicalDevice vk_physical_device = nullptr;
};

constexpr GPU::Kind operator|(GPU::Kind left_a, GPU::Kind right_a)
{
    return static_cast<GPU::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr GPU::Kind operator&(GPU::Kind left_a, GPU::Kind right_a)
{
    return static_cast<GPU::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
constexpr GPU::Kind operator|=(GPU::Kind& left_a, GPU::Kind right_a)
{
    left_a = left_a | right_a;
    return left_a;
}
constexpr GPU::Kind operator&=(GPU::Kind& left_a, GPU::Kind right_a)
{
    left_a = left_a & right_a;
    return left_a;
}

constexpr GPU::Feature operator|(GPU::Feature left_a, GPU::Feature right_a)
{
    return static_cast<GPU::Feature>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr GPU::Feature operator&(GPU::Feature left_a, GPU::Feature right_a)
{
    return static_cast<GPU::Feature>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
constexpr GPU::Feature operator|=(GPU::Feature& left_a, GPU::Feature right_a)
{
    left_a = left_a | right_a;
    return left_a;
}
constexpr GPU::Feature operator&=(GPU::Feature& left_a, GPU::Feature right_a)
{
    left_a = left_a & right_a;
    return left_a;
}

constexpr GPU::QueueFamily::Kind operator|(GPU::QueueFamily::Kind left_a, GPU::QueueFamily::Kind right_a)
{
    return static_cast<GPU::QueueFamily::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr GPU::QueueFamily::Kind operator&(GPU::QueueFamily::Kind left_a, GPU::QueueFamily::Kind right_a)
{
    return static_cast<GPU::QueueFamily::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
constexpr GPU::QueueFamily::Kind operator|=(GPU::QueueFamily::Kind& left_a, GPU::QueueFamily::Kind right_a)
{
    left_a = left_a | right_a;
    return left_a;
}
constexpr GPU::QueueFamily::Kind operator&=(GPU::QueueFamily::Kind& left_a, GPU::QueueFamily::Kind right_a)
{
    left_a = left_a & right_a;
    return left_a;
}
} // namespace lx::devices