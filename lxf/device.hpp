#pragma once

/*
 *   Name: device.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <memory>
#include <span>
#include <string>
#include <string_view>

// platform
#include <Windows.h>
#include <vulkan/vulkan.h>

// lxf
#include <lxf/common/Rect.hpp>
#include <lxf/common/bit.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/scalar.hpp>
#include <lxf/common/various.hpp>

namespace lxf {
struct device : private common::non_constructible
{
    struct Display
    {
    public:
        Display()
            : handle(nullptr)
            , bits_per_pixel(0u)
        {
        }
        Display(HMONITOR handle_a,
                common::Uint8 bits_per_pixel_a,
                std::string_view name_a,
                common::Rect<common::Int32, common::Uint32> logical_rect_a,
                common::Rect<common::Int32, common::Uint32> physical_rect_a)
            : logical_rect(logical_rect_a)
            , physical_rect(physical_rect_a)
            , handle(handle_a)
            , bits_per_pixel(bits_per_pixel_a)
            , name(name_a)
        {
        }

        common::Rect<common::Int32, common::Uint32> get_logical_rect() const
        {
            return this->logical_rect;
        }
        common::Rect<common::Int32, common::Uint32> get_physical_rect() const
        {
            return this->physical_rect;
        }
        common::Uint8 get_bits_per_pixel() const
        {
            return this->bits_per_pixel;
        }
        std::string_view get_name() const
        {
            return this->name;
        }

    private:
        common::Rect<common::Int32, common::Uint32> logical_rect;
        common::Rect<common::Int32, common::Uint32> physical_rect;
        common::Uint8 bits_per_pixel;

        HMONITOR handle;

        std::string name;
    };
    struct GPU
    {
        enum class Kind : common::Uint64
        {
            software = 0x1u,
            discrete = 0x2u,
            integrated = 0x4u,
            indirect = 0x8u,
            primary = 0x10u
        };
        enum class Feature : common::Uint64
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
            enum class Sample_count : common::Uint32
            {
                _1 = VK_SAMPLE_COUNT_1_BIT,
                _2 = VK_SAMPLE_COUNT_2_BIT,
                _4 = VK_SAMPLE_COUNT_4_BIT,
                _8 = VK_SAMPLE_COUNT_8_BIT,
                _16 = VK_SAMPLE_COUNT_16_BIT,
                _32 = VK_SAMPLE_COUNT_32_BIT,
                _64 = VK_SAMPLE_COUNT_64_BIT
            };

            common::Uint32 max_image_dimension_1d = 0u;
            common::Uint32 max_image_dimension_2d = 0u;
            common::Uint32 max_image_dimension_3d = 0u;
            common::Uint32 max_image_dimensioncube = 0u;
            common::Uint32 max_image_arraylayers = 0u;
            common::Uint32 max_texel_buffer_elements = 0u;
            common::Uint32 max_uniform_buffer_range = 0u;
            common::Uint32 max_storage_buffer_range = 0u;
            common::Uint32 max_push_constants_size = 0u;
            common::Uint32 max_memory_allocation_count = 0u;
            common::Uint32 max_sampler_allocation_count = 0u;
            common::Uint64 buffer_image_granularity = 0ull;
            common::Uint64 sparse_address_space_size = 0ull;
            common::Uint32 max_bound_descriptor_sets = 0u;
            common::Uint32 max_per_stage_descriptor_samplers = 0u;
            common::Uint32 max_per_stage_descriptor_uniform_buffers = 0u;
            common::Uint32 max_per_stage_descriptor_storage_buffers = 0u;
            common::Uint32 max_per_stage_descriptor_sampled_images = 0u;
            common::Uint32 max_per_stage_descriptor_storage_images = 0u;
            common::Uint32 max_per_stage_descriptor_input_attachments = 0u;
            common::Uint32 max_per_stage_resources = 0u;
            common::Uint32 max_descriptor_set_samplers = 0u;
            common::Uint32 max_descriptor_set_uniform_buffers = 0u;
            common::Uint32 max_descriptor_set_uniform_buffers_dynamic = 0u;
            common::Uint32 max_descriptor_set_storage_buffers = 0u;
            common::Uint32 max_descriptor_set_storage_buffers_dynamic = 0u;
            common::Uint32 max_descriptor_set_sampled_images = 0u;
            common::Uint32 max_descriptor_set_storage_images = 0u;
            common::Uint32 max_descriptor_set_input_attachments = 0u;
            common::Uint32 max_vertex_input_attributes = 0u;
            common::Uint32 max_vertex_input_bindings = 0u;
            common::Uint32 max_vertex_input_attribute_offset = 0u;
            common::Uint32 max_vertex_input_binding_stride = 0u;
            common::Uint32 max_vertex_output_components = 0u;
            common::Uint32 max_tessellation_generationlevel = 0u;
            common::Uint32 max_tessellation_patchsize = 0u;
            common::Uint32 max_tessellation_control_per_vertex_input_components = 0u;
            common::Uint32 max_tessellation_control_per_vertex_output_components = 0u;
            common::Uint32 max_tessellation_control_per_patch_output_components = 0u;
            common::Uint32 max_tessellation_control_total_output_components = 0u;
            common::Uint32 max_tessellation_evaluation_input_components = 0u;
            common::Uint32 max_tessellation_evaluation_output_components = 0u;
            common::Uint32 max_geometry_shader_invocations = 0u;
            common::Uint32 max_geometry_input_components = 0u;
            common::Uint32 max_geometry_output_components = 0u;
            common::Uint32 max_geometry_output_vertices = 0u;
            common::Uint32 max_geometry_total_output_components = 0u;
            common::Uint32 max_fragment_input_components = 0u;
            common::Uint32 max_fragment_output_attachments = 0u;
            common::Uint32 max_fragment_dual_src_attachments = 0u;
            common::Uint32 max_fragment_combined_output_resources = 0u;
            common::Uint32 max_compute_shared_memory_size = 0u;
            common::Uint32 max_compute_work_group_count[3] = { 0u, 0u, 0u };
            common::Uint32 max_compute_work_group_invocations = 0u;
            common::Uint32 max_compute_work_group_size[3] = { 0u, 0u, 0u };
            common::Uint32 sub_pixel_precision_bits = 0u;
            common::Uint32 sub_texel_precision_bits = 0u;
            common::Uint32 mipmap_precision_bits = 0u;
            common::Uint32 max_draw_indexed_index_value = 0u;
            common::Uint32 max_draw_indirect_count = 0u;
            common::Float32 max_sampler_lod_bias = 0.0f;
            common::Float32 max_sampler_anisotropy = 0.0f;
            common::Uint32 max_viewports = 0u;
            common::Uint32 max_viewport_dimensions[2] = { 0u, 0u };
            common::Float32 viewport_bounds_range[2] = { 0.0f, 0.0f };
            common::Uint32 viewport_sub_pixel_bits = 0u;
            common::Uint64 min_memory_map_alignment = 0ull;
            common::Uint64 min_texel_buffer_offset_alignment = 0ull;
            common::Uint64 min_uniform_buffer_offset_alignment = 0ull;
            common::Uint64 min_storage_buffer_offset_alignment = 0ull;
            common::Int32 min_texel_offset = 0;
            common::Uint32 max_texel_offset = 0u;
            common::Int32 min_texel_gather_offset = 0;
            common::Uint32 max_texel_gather_offset = 0u;
            common::Float32 min_interpolation_offset = 0.0f;
            common::Float32 max_interpolation_offset = 0.0f;
            common::Uint32 sub_pixel_interpolation_offset_bits = 0u;
            common::Uint32 max_framebuffer_width = 0u;
            common::Uint32 max_framebuffer_height = 0u;
            common::Uint32 max_framebuffer_layers = 0u;
            Sample_count framebuffer_color_sample_counts;
            Sample_count framebuffer_depth_sample_counts;
            Sample_count framebuffer_stencil_sample_counts;
            Sample_count framebuffer_no_attachments_sample_counts;
            common::Uint32 max_color_attachments = 0u;
            Sample_count sampled_image_color_sample_counts;
            Sample_count sampled_image_integer_sample_counts;
            Sample_count sampled_image_depth_sample_counts;
            Sample_count sampled_image_stencil_sample_counts;
            Sample_count storage_image_sample_counts;
            common::Uint32 max_sample_mask_words = 0u;
            bool timestamp_compute_and_graphics = false;
            common::Float32 timestamp_period = 0.0f;
            common::Uint32 max_clip_distances = 0u;
            common::Uint32 max_cull_distances = 0u;
            common::Uint32 max_combined_clip_and_cull_distances = 0u;
            common::Uint32 discrete_queue_priorities = 0u;
            common::Float32 point_size_range[2] = { 0.0f, 0.0f };
            common::Float32 line_width_hrange[2] = { 0.0f, 0.0f };
            common::Float32 point_size_granularity = 0.0f;
            common::Float32 line_width_granularity = 0.0f;
            bool strict_lines = false;
            bool standard_sample_locations = false;
            common::Uint64 optimal_buffer_copy_offset_alignment = 0ull;
            common::Uint64 optimal_buffer_copy_row_pitch_alignment = 0ull;
            common::Uint64 non_coherent_atom_size = 0ull;
        };
        struct Queue
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
                optical_flow = VK_QUEUE_OPTICAL_FLOW_BIT_NV
            };

            using enum Kind;

            Kind kind;
            std::uint32_t count;
        };

        struct Descriptor
        {
            Kind kind;
            Feature features;
            Limits limits;
            std::span<const Queue> queue_families;
            std::string_view name;
        };

        using enum Kind;

        GPU(const GPU& other_a)
            : vk_physical_device(other_a.vk_physical_device)
            , kind(other_a.kind)
            , features(other_a.features)
            , limits(other_a.limits)
            , queues { std::make_unique<Queue[]>(other_a.queues_count) }
            , queues_count(other_a.queues_count)
        {
            for (std::size_t i = 0; i < this->queues_count; i++)
            {
                this->queues[i] = { .kind = other_a.queues[i].kind, .count = other_a.queues[i].count };
            }

            std::memcpy(this->name, other_a.name, sizeof(this->name));
        }

        GPU(VkPhysicalDevice vk_physical_device_a,
            VkPhysicalDeviceType type_a,
            const VkPhysicalDeviceLimits& limits_a,
            const VkPhysicalDeviceFeatures& features_a,
            std::span<VkQueueFamilyProperties> queue_family_properties_a,
            bool is_primary_a,
            std::string_view name_a)
            : vk_physical_device(vk_physical_device_a)
            , kind(static_cast<Kind>(static_cast<common::Uint64>(this->from(type_a)) |
                                     (true == is_primary_a ? static_cast<common::Uint64>(Kind::primary) : 0x0ull)))
            , features(this->from(features_a))
            , limits(this->from(limits_a))
            , queues { std::make_unique<Queue[]>(queue_family_properties_a.size()) }
            , queues_count(queue_family_properties_a.size())
        {
            for (std::size_t i = 0; i < this->queues_count; i++)
            {
                this->queues[i].kind = static_cast<Queue::Kind>(queue_family_properties_a[i].queueFlags);
                this->queues[i].count = queue_family_properties_a[i].queueCount;
            }

            static_assert(VK_MAX_PHYSICAL_DEVICE_NAME_SIZE > 1u);

            const std::size_t name_length =
                name_a.length() >= VK_MAX_PHYSICAL_DEVICE_NAME_SIZE - 1u ? VK_MAX_PHYSICAL_DEVICE_NAME_SIZE - 1u : name_a.length();
            std::memcpy(this->name, name_a.data(), name_length);
            this->name[name_length] = '\0';
        }

        Descriptor get_descriptor() const
        {
            return { .kind = this->kind,
                     .features = this->features,
                     .limits = this->limits,
                     .queue_families = { this->queues.get(), this->queues_count },
                     .name = this->name };
        }

        GPU& operator=(const GPU& other_a)
        {
            this->vk_physical_device = other_a.vk_physical_device;
            this->kind = other_a.kind;
            this->features = other_a.features;
            this->limits = other_a.limits;

            this->queues_count = other_a.queues_count;
            for (std::size_t i = 0; i < this->queues_count; i++)
            {
                this->queues[i].kind = other_a.queues[i].kind;
                this->queues[i].count = other_a.queues[i].count;
            }

            std::memcpy(this->name, other_a.name, sizeof(this->name));

            return *this;
        }

    private:
        Kind from(VkPhysicalDeviceType type_a) const
        {
            switch (type_a)
            {
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    return Kind::software;

                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    return Kind::discrete;

                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    return Kind::integrated;

                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    return Kind::indirect;
            }

            return common::various::get_enum_incorrect_value<Kind>();
        }
        Feature from(const VkPhysicalDeviceFeatures& features_a) const
        {
            common::Uint64 ret = 0x0ull;

            if (VK_TRUE == features_a.robustBufferAccess)
            {
                ret |= static_cast<common::Uint64>(Feature::robust_buffer_access);
            }
            if (VK_TRUE == features_a.fullDrawIndexUint32)
            {
                ret |= static_cast<common::Uint64>(Feature::full_draw_index_uint32);
            }
            if (VK_TRUE == features_a.imageCubeArray)
            {
                ret |= static_cast<common::Uint64>(Feature::image_cube_array);
            }
            if (VK_TRUE == features_a.independentBlend)
            {
                ret |= static_cast<common::Uint64>(Feature::independent_blend);
            }
            if (VK_TRUE == features_a.geometryShader)
            {
                ret |= static_cast<common::Uint64>(Feature::geometry_shader);
            }
            if (VK_TRUE == features_a.tessellationShader)
            {
                ret |= static_cast<common::Uint64>(Feature::tessellation_shader);
            }
            if (VK_TRUE == features_a.sampleRateShading)
            {
                ret |= static_cast<common::Uint64>(Feature::sample_rate_shading);
            }
            if (VK_TRUE == features_a.dualSrcBlend)
            {
                ret |= static_cast<common::Uint64>(Feature::dual_src_blend);
            }
            if (VK_TRUE == features_a.logicOp)
            {
                ret |= static_cast<common::Uint64>(Feature::logic_op);
            }
            if (VK_TRUE == features_a.multiDrawIndirect)
            {
                ret |= static_cast<common::Uint64>(Feature::multi_draw_indirect);
            }
            if (VK_TRUE == features_a.drawIndirectFirstInstance)
            {
                ret |= static_cast<common::Uint64>(Feature::draw_indirect_first_instance);
            }
            if (VK_TRUE == features_a.depthClamp)
            {
                ret |= static_cast<common::Uint64>(Feature::depth_clamp);
            }
            if (VK_TRUE == features_a.depthBiasClamp)
            {
                ret |= static_cast<common::Uint64>(Feature::depth_bias_clamp);
            }
            if (VK_TRUE == features_a.fillModeNonSolid)
            {
                ret |= static_cast<common::Uint64>(Feature::fill_mode_non_nolid);
            }
            if (VK_TRUE == features_a.depthBounds)
            {
                ret |= static_cast<common::Uint64>(Feature::depth_bounds);
            }
            if (VK_TRUE == features_a.wideLines)
            {
                ret |= static_cast<common::Uint64>(Feature::wide_lines);
            }
            if (VK_TRUE == features_a.largePoints)
            {
                ret |= static_cast<common::Uint64>(Feature::large_points);
            }
            if (VK_TRUE == features_a.alphaToOne)
            {
                ret |= static_cast<common::Uint64>(Feature::alpha_to_one);
            }
            if (VK_TRUE == features_a.multiViewport)
            {
                ret |= static_cast<common::Uint64>(Feature::multi_viewport);
            }
            if (VK_TRUE == features_a.samplerAnisotropy)
            {
                ret |= static_cast<common::Uint64>(Feature::sampler_anisotropy);
            }
            if (VK_TRUE == features_a.textureCompressionETC2)
            {
                ret |= static_cast<common::Uint64>(Feature::texture_compression_ETC2);
            }
            if (VK_TRUE == features_a.textureCompressionASTC_LDR)
            {
                ret |= static_cast<common::Uint64>(Feature::texture_compression_ASTC_LDR);
            }
            if (VK_TRUE == features_a.textureCompressionBC)
            {
                ret |= static_cast<common::Uint64>(Feature::texture_compression_BC);
            }
            if (VK_TRUE == features_a.occlusionQueryPrecise)
            {
                ret |= static_cast<common::Uint64>(Feature::occlusion_query_precise);
            }
            if (VK_TRUE == features_a.pipelineStatisticsQuery)
            {
                ret |= static_cast<common::Uint64>(Feature::pipeline_statistics_query);
            }
            if (VK_TRUE == features_a.vertexPipelineStoresAndAtomics)
            {
                ret |= static_cast<common::Uint64>(Feature::vertex_pipeline_stores_andatomics);
            }
            if (VK_TRUE == features_a.fragmentStoresAndAtomics)
            {
                ret |= static_cast<common::Uint64>(Feature::fragment_stores_and_atomics);
            }
            if (VK_TRUE == features_a.shaderTessellationAndGeometryPointSize)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_tessellation_and_geometry_point_size);
            }
            if (VK_TRUE == features_a.shaderImageGatherExtended)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_image_gather_extended);
            }
            if (VK_TRUE == features_a.shaderStorageImageExtendedFormats)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_image_extended_formats);
            }
            if (VK_TRUE == features_a.shaderStorageImageMultisample)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_image_multisample);
            }
            if (VK_TRUE == features_a.shaderStorageImageReadWithoutFormat)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_image_read_without_format);
            }
            if (VK_TRUE == features_a.shaderStorageImageWriteWithoutFormat)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_image_write_without_format);
            }
            if (VK_TRUE == features_a.shaderUniformBufferArrayDynamicIndexing)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_uniform_buffer_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderSampledImageArrayDynamicIndexing)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_buffer_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderStorageBufferArrayDynamicIndexing)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_buffer_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderStorageImageArrayDynamicIndexing)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_storage_image_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderClipDistance)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_clip_distance);
            }
            if (VK_TRUE == features_a.shaderCullDistance)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_cull_distance);
            }
            if (VK_TRUE == features_a.shaderFloat64)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_float64);
            }
            if (VK_TRUE == features_a.shaderInt64)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_int64);
            }
            if (VK_TRUE == features_a.shaderInt16)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_int16);
            }
            if (VK_TRUE == features_a.shaderResourceResidency)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_resource_residency);
            }
            if (VK_TRUE == features_a.shaderResourceMinLod)
            {
                ret |= static_cast<common::Uint64>(Feature::shader_resource_min_lod);
            }
            if (VK_TRUE == features_a.sparseBinding)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_binding);
            }
            if (VK_TRUE == features_a.sparseResidencyBuffer)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_buffer);
            }
            if (VK_TRUE == features_a.sparseResidencyImage2D)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_image2d);
            }
            if (VK_TRUE == features_a.sparseResidencyImage3D)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_image3d);
            }
            if (VK_TRUE == features_a.sparseResidency2Samples)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_2_samples);
            }
            if (VK_TRUE == features_a.sparseResidency4Samples)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_4_samples);
            }
            if (VK_TRUE == features_a.sparseResidency8Samples)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_8_samples);
            }
            if (VK_TRUE == features_a.sparseResidency16Samples)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_16_samples);
            }
            if (VK_TRUE == features_a.sparseResidencyAliased)
            {
                ret |= static_cast<common::Uint64>(Feature::sparse_residency_aliased);
            }
            if (VK_TRUE == features_a.variableMultisampleRate)
            {
                ret |= static_cast<common::Uint64>(Feature::variable_multisample_rate);
            }
            if (VK_TRUE == features_a.inheritedQueries)
            {
                ret |= static_cast<common::Uint64>(Feature::inherited_queries);
            }

            return static_cast<Feature>(ret);
        }
        Limits from(const VkPhysicalDeviceLimits& limits_a)
        {
            return { .max_image_dimension_1d = limits_a.maxImageDimension1D,
                     .max_image_dimension_2d = limits_a.maxImageDimension2D,
                     .max_image_dimension_3d = limits_a.maxImageDimension3D,
                     .max_image_dimensioncube = limits_a.maxImageDimensionCube,
                     .max_image_arraylayers = limits_a.maxImageArrayLayers,
                     .max_texel_buffer_elements = limits_a.maxTexelBufferElements,
                     .max_uniform_buffer_range = limits_a.maxUniformBufferRange,
                     .max_storage_buffer_range = limits_a.maxStorageBufferRange,
                     .max_push_constants_size = limits_a.maxPushConstantsSize,
                     .max_memory_allocation_count = limits_a.maxMemoryAllocationCount,
                     .max_sampler_allocation_count = limits_a.maxSamplerAllocationCount,
                     .buffer_image_granularity = limits_a.bufferImageGranularity,
                     .sparse_address_space_size = limits_a.sparseAddressSpaceSize,
                     .max_bound_descriptor_sets = limits_a.maxBoundDescriptorSets,
                     .max_per_stage_descriptor_samplers = limits_a.maxPerStageDescriptorSamplers,
                     .max_per_stage_descriptor_uniform_buffers = limits_a.maxPerStageDescriptorUniformBuffers,
                     .max_per_stage_descriptor_storage_buffers = limits_a.maxPerStageDescriptorStorageBuffers,
                     .max_per_stage_descriptor_sampled_images = limits_a.maxPerStageDescriptorSampledImages,
                     .max_per_stage_descriptor_storage_images = limits_a.maxPerStageDescriptorStorageImages,
                     .max_per_stage_descriptor_input_attachments = limits_a.maxPerStageDescriptorInputAttachments,
                     .max_per_stage_resources = limits_a.maxPerStageResources,
                     .max_descriptor_set_samplers = limits_a.maxDescriptorSetSamplers,
                     .max_descriptor_set_uniform_buffers = limits_a.maxDescriptorSetUniformBuffers,
                     .max_descriptor_set_uniform_buffers_dynamic = limits_a.maxDescriptorSetUniformBuffersDynamic,
                     .max_descriptor_set_storage_buffers = limits_a.maxDescriptorSetStorageBuffers,
                     .max_descriptor_set_storage_buffers_dynamic = limits_a.maxDescriptorSetStorageBuffersDynamic,
                     .max_descriptor_set_sampled_images = limits_a.maxDescriptorSetSampledImages,
                     .max_descriptor_set_storage_images = limits_a.maxDescriptorSetStorageImages,
                     .max_descriptor_set_input_attachments = limits_a.maxDescriptorSetInputAttachments,
                     .max_vertex_input_attributes = limits_a.maxVertexInputAttributes,
                     .max_vertex_input_bindings = limits_a.maxVertexInputBindings,
                     .max_vertex_input_attribute_offset = limits_a.maxVertexInputAttributeOffset,
                     .max_vertex_input_binding_stride = limits_a.maxVertexInputBindingStride,
                     .max_vertex_output_components = limits_a.maxVertexOutputComponents,
                     .max_tessellation_generationlevel = limits_a.maxTessellationGenerationLevel,
                     .max_tessellation_patchsize = limits_a.maxTessellationPatchSize,
                     .max_tessellation_control_per_vertex_input_components = limits_a.maxTessellationControlPerVertexInputComponents,
                     .max_tessellation_control_per_vertex_output_components = limits_a.maxTessellationControlPerVertexOutputComponents,
                     .max_tessellation_control_per_patch_output_components = limits_a.maxTessellationControlPerPatchOutputComponents,
                     .max_tessellation_control_total_output_components = limits_a.maxTessellationControlTotalOutputComponents,
                     .max_tessellation_evaluation_input_components = limits_a.maxTessellationEvaluationInputComponents,
                     .max_tessellation_evaluation_output_components = limits_a.maxTessellationEvaluationOutputComponents,
                     .max_geometry_shader_invocations = limits_a.maxGeometryShaderInvocations,
                     .max_geometry_input_components = limits_a.maxGeometryInputComponents,
                     .max_geometry_output_components = limits_a.maxGeometryOutputComponents,
                     .max_geometry_output_vertices = limits_a.maxGeometryOutputVertices,
                     .max_geometry_total_output_components = limits_a.maxGeometryTotalOutputComponents,
                     .max_fragment_input_components = limits_a.maxFragmentInputComponents,
                     .max_fragment_output_attachments = limits_a.maxFragmentOutputAttachments,
                     .max_fragment_dual_src_attachments = limits_a.maxFragmentDualSrcAttachments,
                     .max_fragment_combined_output_resources = limits_a.maxFragmentCombinedOutputResources,
                     .max_compute_shared_memory_size = limits_a.maxComputeSharedMemorySize,
                     .max_compute_work_group_count = { limits_a.maxComputeWorkGroupCount[0],
                                                       limits_a.maxComputeWorkGroupCount[1],
                                                       limits_a.maxComputeWorkGroupCount[2] },
                     .max_compute_work_group_invocations = limits_a.maxComputeWorkGroupInvocations,
                     .max_compute_work_group_size = { limits_a.maxComputeWorkGroupSize[0],
                                                      limits_a.maxComputeWorkGroupSize[1],
                                                      limits_a.maxComputeWorkGroupSize[2] },
                     .sub_pixel_precision_bits = limits_a.subPixelPrecisionBits,
                     .sub_texel_precision_bits = limits_a.subTexelPrecisionBits,
                     .mipmap_precision_bits = limits_a.mipmapPrecisionBits,
                     .max_draw_indexed_index_value = limits_a.maxDrawIndexedIndexValue,
                     .max_draw_indirect_count = limits_a.maxDrawIndirectCount,
                     .max_sampler_lod_bias = limits_a.maxSamplerLodBias,
                     .max_sampler_anisotropy = limits_a.maxSamplerAnisotropy,
                     .max_viewports = limits_a.maxViewports,
                     .max_viewport_dimensions = { limits_a.maxViewportDimensions[0], limits_a.maxViewportDimensions[1] },
                     .viewport_bounds_range = { limits_a.viewportBoundsRange[0], limits_a.viewportBoundsRange[1] },
                     .viewport_sub_pixel_bits = limits_a.viewportSubPixelBits,
                     .min_memory_map_alignment = limits_a.minMemoryMapAlignment,
                     .min_texel_buffer_offset_alignment = limits_a.minTexelBufferOffsetAlignment,
                     .min_uniform_buffer_offset_alignment = limits_a.minUniformBufferOffsetAlignment,
                     .min_storage_buffer_offset_alignment = limits_a.minStorageBufferOffsetAlignment,
                     .min_texel_offset = limits_a.minTexelOffset,
                     .max_texel_offset = limits_a.maxTexelOffset,
                     .min_texel_gather_offset = limits_a.minTexelGatherOffset,
                     .max_texel_gather_offset = limits_a.maxTexelGatherOffset,
                     .min_interpolation_offset = limits_a.minInterpolationOffset,
                     .max_interpolation_offset = limits_a.maxInterpolationOffset,
                     .sub_pixel_interpolation_offset_bits = limits_a.subPixelInterpolationOffsetBits,
                     .max_framebuffer_width = limits_a.maxFramebufferWidth,
                     .max_framebuffer_height = limits_a.maxFramebufferHeight,
                     .max_framebuffer_layers = limits_a.maxFramebufferLayers,
                     .framebuffer_color_sample_counts = static_cast<Limits::Sample_count>(limits_a.framebufferColorSampleCounts),
                     .framebuffer_depth_sample_counts = static_cast<Limits::Sample_count>(limits_a.framebufferDepthSampleCounts),
                     .framebuffer_stencil_sample_counts = static_cast<Limits::Sample_count>(limits_a.framebufferStencilSampleCounts),
                     .framebuffer_no_attachments_sample_counts =
                         static_cast<Limits::Sample_count>(limits_a.framebufferNoAttachmentsSampleCounts),
                     .max_color_attachments = limits_a.maxColorAttachments,
                     .sampled_image_color_sample_counts = static_cast<Limits::Sample_count>(limits_a.sampledImageColorSampleCounts),
                     .sampled_image_integer_sample_counts = static_cast<Limits::Sample_count>(limits_a.sampledImageIntegerSampleCounts),
                     .sampled_image_depth_sample_counts = static_cast<Limits::Sample_count>(limits_a.sampledImageDepthSampleCounts),
                     .sampled_image_stencil_sample_counts = static_cast<Limits::Sample_count>(limits_a.sampledImageStencilSampleCounts),
                     .storage_image_sample_counts = static_cast<Limits::Sample_count>(limits_a.storageImageSampleCounts),
                     .max_sample_mask_words = limits_a.maxSampleMaskWords,
                     .timestamp_compute_and_graphics = 1u == limits_a.timestampComputeAndGraphics,
                     .timestamp_period = limits_a.timestampPeriod,
                     .max_clip_distances = limits_a.maxClipDistances,
                     .max_cull_distances = limits_a.maxCullDistances,
                     .max_combined_clip_and_cull_distances = limits_a.maxCombinedClipAndCullDistances,
                     .discrete_queue_priorities = limits_a.discreteQueuePriorities,
                     .point_size_range = { limits_a.pointSizeRange[0], limits_a.pointSizeRange[1] },
                     .line_width_hrange = { limits_a.lineWidthRange[0], limits_a.lineWidthRange[1] },
                     .point_size_granularity = limits_a.pointSizeGranularity,
                     .line_width_granularity = limits_a.lineWidthGranularity,
                     .strict_lines = 1u == limits_a.strictLines,
                     .standard_sample_locations = 1u == limits_a.standardSampleLocations,
                     .optimal_buffer_copy_offset_alignment = limits_a.optimalBufferCopyOffsetAlignment,
                     .optimal_buffer_copy_row_pitch_alignment = limits_a.optimalBufferCopyRowPitchAlignment,
                     .non_coherent_atom_size = limits_a.nonCoherentAtomSize };
        }

        VkPhysicalDevice vk_physical_device;

        Kind kind;
        Feature features;
        Limits limits;

        std::unique_ptr<Queue[]> queues;
        std::size_t queues_count;

        char name[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
    };
    struct Input
    {
        enum class Kind : common::Uint64
        {
            keyboard = 0x1u,
            mouse = 0x2u,
            pad = 0x4u
        };

        Input(Kind kind_a)
            : kind(kind_a)
        {
        }

        Kind get_kind() const
        {
            return this->kind;
        }

    private:
        Kind kind;
    };

    template<typename Device_t>
    [[nodiscard]] static std::vector<Device_t> filter(const std::vector<device::Display>& displays_a,
                                                      const common::Size<common::Uint32>& minimum_physical_resolution_a,
                                                      common::Uint8 bits_per_pixel_a) = delete;
    template<typename Device_t> [[nodiscard]] static std::vector<Device_t> filter(Input::Kind) = delete;
    template<typename Device_t> [[nodiscard]] static std::vector<Device_t>
    filter(const std::vector<GPU>& gpus_a, GPU::Kind, GPU::Queue::Kind, GPU::Feature) = delete;
};

template<>
[[nodiscard]] inline std::vector<device::Display> device::filter(const std::vector<device::Display>& displays_a,
                                                                 const common::Size<common::Uint32>& minimum_physical_resolution_a,
                                                                 common::Uint8 bits_per_pixel_a)
{
    std::vector<Display> ret;

    for (const Display& device : displays_a)
    {
        const auto phy_size = device.get_physical_rect().size;
        if (phy_size.w >= minimum_physical_resolution_a.w && phy_size.h >= minimum_physical_resolution_a.h &&
            device.get_bits_per_pixel() >= bits_per_pixel_a)
        {
            ret.push_back(device);
        }
    }

    return ret;
}

template<> inline std::vector<device::GPU> [[nodiscard]] device::filter<device::GPU>(const std::vector<GPU>& gpus_a,
                                                                                     GPU::Kind gpu_kind_flags_a,
                                                                                     GPU::Queue::Kind queue_family_kind_a,
                                                                                     GPU::Feature features_a)
{
    std::vector<GPU> ret;

    for (const GPU& device : gpus_a)
    {
        GPU::Descriptor descriptor = device.get_descriptor();
        if (true == common::bit::flag::is(descriptor.kind, gpu_kind_flags_a) &&
            true == common::bit::flag::is(descriptor.features, features_a))
        {
            for (const GPU::Queue& qf : descriptor.queue_families)
            {
                if (true == common::bit::flag::is(qf.kind, queue_family_kind_a))
                {
                    ret.push_back(device);
                    break;
                }
            }
        }
    }

    return ret;
}

constexpr device::GPU::Kind operator|(device::GPU::Kind left_a, device::GPU::Kind right_a)
{
    return static_cast<device::GPU::Kind>(static_cast<common::Uint64>(left_a) | static_cast<common::Uint64>(right_a));
}
constexpr device::GPU::Kind operator&(device::GPU::Kind left_a, device::GPU::Kind right_a)
{
    return static_cast<device::GPU::Kind>(static_cast<common::Uint64>(left_a) & static_cast<common::Uint64>(right_a));
}

constexpr device::GPU::Feature operator|(device::GPU::Feature left_a, device::GPU::Feature right_a)
{
    return static_cast<device::GPU::Feature>(static_cast<common::Uint64>(left_a) | static_cast<common::Uint64>(right_a));
}
constexpr device::GPU::Feature operator&(device::GPU::Feature left_a, device::GPU::Feature right_a)
{
    return static_cast<device::GPU::Feature>(static_cast<common::Uint64>(left_a) & static_cast<common::Uint64>(right_a));
}

constexpr device::GPU::Queue::Kind operator|(device::GPU::Queue::Kind left_a, device::GPU::Queue::Kind right_a)
{
    return static_cast<device::GPU::Queue::Kind>(static_cast<common::Uint64>(left_a) | static_cast<common::Uint64>(right_a));
}
constexpr device::GPU::Queue::Kind operator&(device::GPU::Queue::Kind left_a, device::GPU::Queue::Kind right_a)
{
    return static_cast<device::GPU::Queue::Kind>(static_cast<common::Uint64>(left_a) & static_cast<common::Uint64>(right_a));
}

constexpr device::GPU::Limits::Sample_count operator|(device::GPU::Limits::Sample_count left_a, device::GPU::Limits::Sample_count right_a)
{
    return static_cast<device::GPU::Limits::Sample_count>(static_cast<common::Uint64>(left_a) | static_cast<common::Uint64>(right_a));
}
constexpr device::GPU::Limits::Sample_count operator&(device::GPU::Limits::Sample_count left_a, device::GPU::Limits::Sample_count right_a)
{
    return static_cast<device::GPU::Limits::Sample_count>(static_cast<common::Uint64>(left_a) & static_cast<common::Uint64>(right_a));
}
} // namespace lxf