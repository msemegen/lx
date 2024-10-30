#pragma once

/*
 *   Name: device.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <array>
#include <cassert>
#include <memory>
#include <span>
#include <string>
#include <string_view>

// lxf
#include <lxf/canvas.hpp>
#include <lxf/common/Rect.hpp>
#include <lxf/common/bit.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/common/various.hpp>
#include <lxf/loader/vulkan.hpp>

namespace lxf {
struct device : private common::non_constructible
{
    struct GPU : private common::non_copyable
    {
        struct Properties
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
            struct Queue_family
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
                    presentation = 0x200u
                };

                using enum Kind;

                Kind kind;
                std::size_t count;
                std::size_t index;
            };

            Kind kind;
            Feature features;
            Limits limits;

            std::span<const Queue_family> queue_families;
            std::vector<const char*> extensions;
            const char* p_name;
        };

        using enum Properties::Kind;

        GPU() = default;
        GPU(GPU&&) = default;

        GPU(VkPhysicalDevice vk_physical_device_a,
            bool is_primary_a,
            std::span<VkQueueFamilyProperties> queues_a,
            std::string_view name_a,
            const std::vector<std::string_view>& extension_names_a);

        Properties get_properties() const;

        operator VkPhysicalDevice() const
        {
            return this->vk_physical_device;
        }

    private:
        struct Info
        {
            struct Layout
            {
                struct Range
                {
                    std::size_t offset_bytes = 0u;
                    std::size_t size_bytes = 0u;
                };

                Range name_buffer;
                Range queue_families_buffer;
                Range extensions_buffer;
                Range flags_buffer;
            };

            Layout layout;
            std::unique_ptr<std::byte[]> buffer;

            std::span<Properties::Queue_family> get_queue_families() const
            {
                assert(this->layout.queue_families_buffer.size_bytes >= sizeof(Properties::Queue_family));

                return { std::bit_cast<Properties::Queue_family*>(this->buffer.get() + this->layout.queue_families_buffer.offset_bytes),
                         static_cast<std::size_t>(this->layout.queue_families_buffer.size_bytes / sizeof(Properties::Queue_family)) };
            }

            const char* get_name() const
            {
                assert(this->layout.name_buffer.size_bytes > 0u);

                return std::bit_cast<const char*>(this->buffer.get() + this->layout.name_buffer.offset_bytes);
            }
        };

        Properties::Kind from(VkPhysicalDeviceType type_a) const
        {
            switch (type_a)
            {
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    return Properties::Kind::software;

                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    return Properties::Kind::discrete;

                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    return Properties::Kind::integrated;

                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    return Properties::Kind::indirect;
            }

            return common::various::get_enum_incorrect_value<Properties::Kind>();
        }
        Properties::Feature from(const VkPhysicalDeviceFeatures& features_a) const
        {
            std::uint64_t ret = 0x0ull;

            if (VK_TRUE == features_a.robustBufferAccess)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::robust_buffer_access);
            }
            if (VK_TRUE == features_a.fullDrawIndexUint32)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::full_draw_index_uint32);
            }
            if (VK_TRUE == features_a.imageCubeArray)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::image_cube_array);
            }
            if (VK_TRUE == features_a.independentBlend)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::independent_blend);
            }
            if (VK_TRUE == features_a.geometryShader)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::geometry_shader);
            }
            if (VK_TRUE == features_a.tessellationShader)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::tessellation_shader);
            }
            if (VK_TRUE == features_a.sampleRateShading)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sample_rate_shading);
            }
            if (VK_TRUE == features_a.dualSrcBlend)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::dual_src_blend);
            }
            if (VK_TRUE == features_a.logicOp)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::logic_op);
            }
            if (VK_TRUE == features_a.multiDrawIndirect)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::multi_draw_indirect);
            }
            if (VK_TRUE == features_a.drawIndirectFirstInstance)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::draw_indirect_first_instance);
            }
            if (VK_TRUE == features_a.depthClamp)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::depth_clamp);
            }
            if (VK_TRUE == features_a.depthBiasClamp)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::depth_bias_clamp);
            }
            if (VK_TRUE == features_a.fillModeNonSolid)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::fill_mode_non_nolid);
            }
            if (VK_TRUE == features_a.depthBounds)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::depth_bounds);
            }
            if (VK_TRUE == features_a.wideLines)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::wide_lines);
            }
            if (VK_TRUE == features_a.largePoints)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::large_points);
            }
            if (VK_TRUE == features_a.alphaToOne)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::alpha_to_one);
            }
            if (VK_TRUE == features_a.multiViewport)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::multi_viewport);
            }
            if (VK_TRUE == features_a.samplerAnisotropy)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sampler_anisotropy);
            }
            if (VK_TRUE == features_a.textureCompressionETC2)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::texture_compression_ETC2);
            }
            if (VK_TRUE == features_a.textureCompressionASTC_LDR)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::texture_compression_ASTC_LDR);
            }
            if (VK_TRUE == features_a.textureCompressionBC)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::texture_compression_BC);
            }
            if (VK_TRUE == features_a.occlusionQueryPrecise)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::occlusion_query_precise);
            }
            if (VK_TRUE == features_a.pipelineStatisticsQuery)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::pipeline_statistics_query);
            }
            if (VK_TRUE == features_a.vertexPipelineStoresAndAtomics)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::vertex_pipeline_stores_andatomics);
            }
            if (VK_TRUE == features_a.fragmentStoresAndAtomics)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::fragment_stores_and_atomics);
            }
            if (VK_TRUE == features_a.shaderTessellationAndGeometryPointSize)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_tessellation_and_geometry_point_size);
            }
            if (VK_TRUE == features_a.shaderImageGatherExtended)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_image_gather_extended);
            }
            if (VK_TRUE == features_a.shaderStorageImageExtendedFormats)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_image_extended_formats);
            }
            if (VK_TRUE == features_a.shaderStorageImageMultisample)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_image_multisample);
            }
            if (VK_TRUE == features_a.shaderStorageImageReadWithoutFormat)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_image_read_without_format);
            }
            if (VK_TRUE == features_a.shaderStorageImageWriteWithoutFormat)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_image_write_without_format);
            }
            if (VK_TRUE == features_a.shaderUniformBufferArrayDynamicIndexing)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_uniform_buffer_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderSampledImageArrayDynamicIndexing)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_buffer_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderStorageBufferArrayDynamicIndexing)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_buffer_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderStorageImageArrayDynamicIndexing)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_storage_image_array_dynamic_indexing);
            }
            if (VK_TRUE == features_a.shaderClipDistance)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_clip_distance);
            }
            if (VK_TRUE == features_a.shaderCullDistance)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_cull_distance);
            }
            if (VK_TRUE == features_a.shaderFloat64)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_float64);
            }
            if (VK_TRUE == features_a.shaderInt64)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_int64);
            }
            if (VK_TRUE == features_a.shaderInt16)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_int16);
            }
            if (VK_TRUE == features_a.shaderResourceResidency)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_resource_residency);
            }
            if (VK_TRUE == features_a.shaderResourceMinLod)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::shader_resource_min_lod);
            }
            if (VK_TRUE == features_a.sparseBinding)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_binding);
            }
            if (VK_TRUE == features_a.sparseResidencyBuffer)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_buffer);
            }
            if (VK_TRUE == features_a.sparseResidencyImage2D)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_image2d);
            }
            if (VK_TRUE == features_a.sparseResidencyImage3D)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_image3d);
            }
            if (VK_TRUE == features_a.sparseResidency2Samples)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_2_samples);
            }
            if (VK_TRUE == features_a.sparseResidency4Samples)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_4_samples);
            }
            if (VK_TRUE == features_a.sparseResidency8Samples)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_8_samples);
            }
            if (VK_TRUE == features_a.sparseResidency16Samples)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_16_samples);
            }
            if (VK_TRUE == features_a.sparseResidencyAliased)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::sparse_residency_aliased);
            }
            if (VK_TRUE == features_a.variableMultisampleRate)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::variable_multisample_rate);
            }
            if (VK_TRUE == features_a.inheritedQueries)
            {
                ret |= static_cast<std::uint64_t>(Properties::Feature::inherited_queries);
            }

            return static_cast<Properties::Feature>(ret);
        }
        Properties::Limits from(const VkPhysicalDeviceLimits& limits_a) const
        {
            return {
                .max_image_dimension_1d = limits_a.maxImageDimension1D,
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
                .framebuffer_color_sample_counts = static_cast<Properties::Limits::Sample_count>(limits_a.framebufferColorSampleCounts),
                .framebuffer_depth_sample_counts = static_cast<Properties::Limits::Sample_count>(limits_a.framebufferDepthSampleCounts),
                .framebuffer_stencil_sample_counts = static_cast<Properties::Limits::Sample_count>(limits_a.framebufferStencilSampleCounts),
                .framebuffer_no_attachments_sample_counts =
                    static_cast<Properties::Limits::Sample_count>(limits_a.framebufferNoAttachmentsSampleCounts),
                .max_color_attachments = limits_a.maxColorAttachments,
                .sampled_image_color_sample_counts = static_cast<Properties::Limits::Sample_count>(limits_a.sampledImageColorSampleCounts),
                .sampled_image_integer_sample_counts =
                    static_cast<Properties::Limits::Sample_count>(limits_a.sampledImageIntegerSampleCounts),
                .sampled_image_depth_sample_counts = static_cast<Properties::Limits::Sample_count>(limits_a.sampledImageDepthSampleCounts),
                .sampled_image_stencil_sample_counts =
                    static_cast<Properties::Limits::Sample_count>(limits_a.sampledImageStencilSampleCounts),
                .storage_image_sample_counts = static_cast<Properties::Limits::Sample_count>(limits_a.storageImageSampleCounts),
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
                .non_coherent_atom_size = limits_a.nonCoherentAtomSize
            };
        }

        VkPhysicalDevice vk_physical_device;
        std::size_t info_idx;

        static inline std::vector<Info> gpu_info_buffer;
    };
    struct Display : private common::non_copyable
    {
        struct Properties
        {
            enum class Kind : std::uint32_t
            {
                primary,
                additional
            };

            Kind kind;

            common::Rect<std::int32_t, std::uint32_t> logical_rect;
            common::Rect<std::int32_t, std::uint32_t> physical_rect;
            std::uint8_t bits_per_pixel;

            char name[CCHDEVICENAME];
        };

        using enum Properties::Kind;

        Display()
            : handle(nullptr)
        {
        }
        Display(HMONITOR handle_a,
                bool is_primary_a,
                std::uint8_t bits_per_pixel_a,
                std::string_view name_a,
                common::Rect<std::int32_t, std::uint32_t> logical_rect_a,
                common::Rect<std::int32_t, std::uint32_t> physical_rect_a)
            : handle(handle_a)
            , properties { .kind = true == is_primary_a ? Properties::Kind::primary : Properties::Kind::additional,
                           .logical_rect = logical_rect_a,
                           .physical_rect = physical_rect_a,
                           .bits_per_pixel = bits_per_pixel_a }
        {
            const std::size_t name_length = name_a.size() <= CCHDEVICENAME ? name_a.size() : CCHDEVICENAME;

            std::fill(this->properties.name, this->properties.name + CCHDEVICENAME, '\0');
            std::copy(name_a.begin(), name_a.begin() + name_length, this->properties.name);
        }

        const Properties& get_properties() const
        {
            return this->properties;
        }

    private:
        HMONITOR handle;
        Properties properties;
    };

    inline static struct filter
    {
        struct GPU
        {
            enum class Kind : std::uint64_t
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

            struct Limit
            {
                enum class Kind : std::uint64_t
                {
                    max_image_dimension_1d = 1ull,
                    max_image_dimension_2d,
                    max_image_dimension_3d,
                    max_image_dimensioncube,
                    max_image_arraylayers,
                    max_texel_buffer_elements,
                    max_uniform_buffer_range,
                    max_storage_buffer_range,
                    max_push_constants_size,
                    max_memory_allocation_count,
                    max_sampler_allocation_count,
                    buffer_image_granularity,
                    sparse_address_space_size,
                    max_bound_descriptor_sets,
                    max_per_stage_descriptor_samplers,
                    max_per_stage_descriptor_uniform_buffers,
                    max_per_stage_descriptor_storage_buffers,
                    max_per_stage_descriptor_sampled_images,
                    max_per_stage_descriptor_storage_images,
                    max_per_stage_descriptor_input_attachments,
                    max_per_stage_resources,
                    max_descriptor_set_samplers,
                    max_descriptor_set_uniform_buffers,
                    max_descriptor_set_uniform_buffers_dynamic,
                    max_descriptor_set_storage_buffers,
                    max_descriptor_set_storage_buffers_dynamic,
                    max_descriptor_set_sampled_images,
                    max_descriptor_set_storage_images,
                    max_descriptor_set_input_attachments,
                    max_vertex_input_attributes,
                    max_vertex_input_bindings,
                    max_vertex_input_attribute_offset,
                    max_vertex_input_binding_stride,
                    max_vertex_output_components,
                    max_tessellation_generationlevel,
                    max_tessellation_patchsize,
                    max_tessellation_control_per_vertex_input_components,
                    max_tessellation_control_per_vertex_output_components,
                    max_tessellation_control_per_patch_output_components,
                    max_tessellation_control_total_output_components,
                    max_tessellation_evaluation_input_components,
                    max_tessellation_evaluation_output_components,
                    max_geometry_shader_invocations,
                    max_geometry_input_components,
                    max_geometry_output_components,
                    max_geometry_output_vertices,
                    max_geometry_total_output_components,
                    max_fragment_input_components,
                    max_fragment_output_attachments,
                    max_fragment_dual_src_attachments,
                    max_fragment_combined_output_resources,
                    max_compute_shared_memory_size,
                    max_compute_work_group_count_0,
                    max_compute_work_group_count_1,
                    max_compute_work_group_count_2,
                    max_compute_work_group_invocations,
                    max_compute_work_group_size_0,
                    max_compute_work_group_size_1,
                    max_compute_work_group_size_2,
                    sub_pixel_precision_bits,
                    sub_texel_precision_bits,
                    mipmap_precision_bits,
                    max_draw_indexed_index_value,
                    max_draw_indirect_count,
                    max_sampler_lod_bias,
                    max_sampler_anisotropy,
                    max_viewports,
                    max_viewport_dimensions_0,
                    max_viewport_dimensions_1,
                    viewport_bounds_range_0,
                    viewport_bounds_range_1,
                    viewport_sub_pixel_bits,
                    min_memory_map_alignment,
                    min_texel_buffer_offset_alignment,
                    min_uniform_buffer_offset_alignment,
                    min_storage_buffer_offset_alignment,
                    min_texel_offset,
                    max_texel_offset,
                    min_texel_gather_offset,
                    max_texel_gather_offset,
                    min_interpolation_offset,
                    max_interpolation_offset,
                    sub_pixel_interpolation_offset_bits,
                    max_framebuffer_width,
                    max_framebuffer_height,
                    max_framebuffer_layers,
                    framebuffer_color_sample_counts,
                    framebuffer_depth_sample_counts,
                    framebuffer_stencil_sample_counts,
                    framebuffer_no_attachments_sample_counts,
                    max_color_attachments,
                    sampled_image_color_sample_counts,
                    sampled_image_integer_sample_counts,
                    sampled_image_depth_sample_counts,
                    sampled_image_stencil_sample_counts,
                    storage_image_sample_counts,
                    max_sample_mask_words,
                    timestamp_compute_and_graphics,
                    timestamp_period,
                    max_clip_distances,
                    max_cull_distances,
                    max_combined_clip_and_cull_distances,
                    discrete_queue_priorities,
                    point_size_range_0,
                    point_size_range_1,
                    line_width_hrange_0,
                    line_width_hrange_1,
                    point_size_granularity,
                    line_width_granularity,
                    strict_lines,
                    standard_sample_locations,
                    optimal_buffer_copy_offset_alignment,
                    optimal_buffer_copy_row_pitch_alignment,
                    non_coherent_atom_size,
                };

                struct Value
                {
                private:
                    enum class Operation : std::uint64_t
                    {
                        less,
                        less_or_equal,
                        greater,
                        greater_or_equal,
                        equals
                    };

                    Operation operation;
                    std::byte data[8];

                    friend Limit;
                    friend device;

                    template<typename Value_x> friend bool check_limit(const Limit req_limit_a, Value_x value_a);
                };

                using enum Kind;

                Kind kind;
                Value value;

                static constexpr Value less(std::uint32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less(std::int32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less(std::uint64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less(std::int64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less(float v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(&(ret.data[0]), &(ret.data[sizeof(v_a) - 1u]), std::bit_cast<std::byte*>(&v_a));

                    return ret;
                }

                static constexpr Value less_or_equal(std::uint32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less_or_equal(std::int32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less_or_equal(std::uint64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less_or_equal(std::int64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value less_or_equal(float v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::less_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(&(ret.data[0]), &(ret.data[sizeof(v_a) - 1u]), std::bit_cast<std::byte*>(&v_a));

                    return ret;
                }

                static constexpr Value greater(std::uint32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater(std::int32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater(std::uint64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater(std::int64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater(float v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(&(ret.data[0]), &(ret.data[sizeof(v_a) - 1u]), std::bit_cast<std::byte*>(&v_a));

                    return ret;
                }

                static constexpr Value greater_or_equal(std::uint32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater_or_equal(std::int32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater_or_equal(std::uint64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater_or_equal(std::int64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value greater_or_equal(float v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::greater_or_equal;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(&(ret.data[0]), &(ret.data[sizeof(v_a) - 1u]), std::bit_cast<std::byte*>(&v_a));

                    return ret;
                }

                static constexpr Value equals(std::uint32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::equals;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value equals(std::int32_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::equals;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value equals(std::uint64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::equals;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }
                static constexpr Value equals(std::int64_t v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::equals;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(std::bit_cast<std::byte*>(&v_a), std::bit_cast<std::byte*>(&v_a) + sizeof(v_a), ret.data);

                    return ret;
                }

                static constexpr Value equals(float v_a)
                {
                    static_assert(sizeof(Value::data) >= sizeof(v_a));

                    Value ret;
                    ret.operation = Value::Operation::equals;
                    std::fill(&(ret.data[0]), &(ret.data[sizeof(ret.data) - 1u]), std::byte { 0x0u });
                    std::copy(&(ret.data[0]), &(ret.data[sizeof(v_a) - 1u]), std::bit_cast<std::byte*>(&v_a));

                    return ret;
                }
            };
            struct Queue_family
            {
                enum class Kind : std::uint64_t
                {
                    graphics = VK_QUEUE_GRAPHICS_BIT,
                    compute = VK_QUEUE_COMPUTE_BIT,
                    transfer = VK_QUEUE_TRANSFER_BIT,
                    sparse_binding = VK_QUEUE_SPARSE_BINDING_BIT,
                    protected_memory = VK_QUEUE_PROTECTED_BIT,
                    video_decode = VK_QUEUE_VIDEO_DECODE_BIT_KHR,
                    video_encode = VK_QUEUE_VIDEO_ENCODE_BIT_KHR,
                    optical_flow = VK_QUEUE_OPTICAL_FLOW_BIT_NV,
                    presentation = 0x200u
                };

                using enum Kind;

                Kind kind;
                std::uint32_t count;
            };
            struct Swap_chain
            {
                enum class Pixel : std::uint64_t
                {
                    undefined = VK_FORMAT_UNDEFINED,
                    r4g4_unorm_pack8 = VK_FORMAT_R4G4_UNORM_PACK8,
                    r4g4b4a4_unorm_pack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
                    b4g4r4a4_unorm_pack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
                    r5g6b5_unorm_pack16 = VK_FORMAT_R5G6B5_UNORM_PACK16,
                    b5g6r5_unorm_pack16 = VK_FORMAT_B5G6R5_UNORM_PACK16,
                    r5g5b5a1_unorm_pack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
                    b5g5r5a1_unorm_pack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
                    a1r5g5b5_unorm_pack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
                    r8_unorm = VK_FORMAT_R8_UNORM,
                    r8_snorm = VK_FORMAT_R8_SNORM,
                    r8_uscaled = VK_FORMAT_R8_USCALED,
                    r8_sscaled = VK_FORMAT_R8_SSCALED,
                    r8_uint = VK_FORMAT_R8_UINT,
                    r8_sint = VK_FORMAT_R8_SINT,
                    r8_srgb = VK_FORMAT_R8_SRGB,
                    r8g8_unorm = VK_FORMAT_R8G8_UNORM,
                    r8g8_snorm = VK_FORMAT_R8G8_SNORM,
                    r8g8_uscaled = VK_FORMAT_R8G8_USCALED,
                    r8g8_sscaled = VK_FORMAT_R8G8_SSCALED,
                    r8g8_uint = VK_FORMAT_R8G8_UINT,
                    r8g8_sint = VK_FORMAT_R8G8_SINT,
                    r8g8_srgb = VK_FORMAT_R8G8_SRGB,
                    r8g8b8_unorm = VK_FORMAT_R8G8B8_UNORM,
                    r8g8b8_snorm = VK_FORMAT_R8G8B8_SNORM,
                    r8g8b8_uscaled = VK_FORMAT_R8G8B8_USCALED,
                    r8g8b8_sscaled = VK_FORMAT_R8G8B8_SSCALED,
                    r8g8b8_uint = VK_FORMAT_R8G8B8_UINT,
                    r8g8b8_sint = VK_FORMAT_R8G8B8_SINT,
                    r8g8b8_srgb = VK_FORMAT_R8G8B8_SRGB,
                    b8g8r8_unorm = VK_FORMAT_B8G8R8_UNORM,
                    b8g8r8_snorm = VK_FORMAT_B8G8R8_SNORM,
                    b8g8r8_uscaled = VK_FORMAT_B8G8R8_USCALED,
                    b8g8r8_sscaled = VK_FORMAT_B8G8R8_SSCALED,
                    b8g8r8_uint = VK_FORMAT_B8G8R8_UINT,
                    b8g8r8_sint = VK_FORMAT_B8G8R8_SINT,
                    b8g8r8_srgb = VK_FORMAT_B8G8R8_SRGB,
                    r8g8b8a8_unorm = VK_FORMAT_R8G8B8A8_UNORM,
                    r8g8b8a8_snorm = VK_FORMAT_R8G8B8A8_SNORM,
                    r8g8b8a8_uscaled = VK_FORMAT_R8G8B8A8_USCALED,
                    r8g8b8a8_sscaled = VK_FORMAT_R8G8B8A8_SSCALED,
                    r8g8b8a8_uint = VK_FORMAT_R8G8B8A8_UINT,
                    r8g8b8a8_sint = VK_FORMAT_R8G8B8A8_SINT,
                    r8g8b8a8_srgb = VK_FORMAT_R8G8B8A8_SRGB,
                    b8g8r8a8_unorm = VK_FORMAT_B8G8R8A8_UNORM,
                    b8g8r8a8_snorm = VK_FORMAT_B8G8R8A8_SNORM,
                    b8g8r8a8_uscaled = VK_FORMAT_B8G8R8A8_USCALED,
                    b8g8r8a8_sscaled = VK_FORMAT_B8G8R8A8_SSCALED,
                    b8g8r8a8_uint = VK_FORMAT_B8G8R8A8_UINT,
                    b8g8r8a8_sint = VK_FORMAT_B8G8R8A8_SINT,
                    b8g8r8a8_srgb = VK_FORMAT_B8G8R8A8_SRGB,
                    a8b8g8r8_unorm_pack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
                    a8b8g8r8_snorm_pack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
                    a8b8g8r8_uscaled_pack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
                    a8b8g8r8_sscaled_pack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
                    a8b8g8r8_uint_pack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32,
                    a8b8g8r8_sint_pack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32,
                    a8b8g8r8_srgb_pack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
                    a2r10g10b10_unorm_pack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
                    a2r10g10b10_snorm_pack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
                    a2r10g10b10_uscaled_pack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
                    a2r10g10b10_sscaled_pack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
                    a2r10g10b10_uint_pack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32,
                    a2r10g10b10_sint_pack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32,
                    a2b10g10r10_unorm_pack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
                    a2b10g10r10_snorm_pack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
                    a2b10g10r10_uscaled_pack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
                    a2b10g10r10_sscaled_pack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
                    a2b10g10r10_uint_pack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32,
                    a2b10g10r10_sint_pack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32,
                    r16_unorm = VK_FORMAT_R16_UNORM,
                    r16_snorm = VK_FORMAT_R16_SNORM,
                    r16_uscaled = VK_FORMAT_R16_USCALED,
                    r16_sscaled = VK_FORMAT_R16_SSCALED,
                    r16_uint = VK_FORMAT_R16_UINT,
                    r16_sint = VK_FORMAT_R16_SINT,
                    r16_sfloat = VK_FORMAT_R16_SFLOAT,
                    r16g16_unorm = VK_FORMAT_R16G16_UNORM,
                    r16g16_snorm = VK_FORMAT_R16G16_SNORM,
                    r16g16_uscaled = VK_FORMAT_R16G16_USCALED,
                    r16g16_sscaled = VK_FORMAT_R16G16_SSCALED,
                    r16g16_uint = VK_FORMAT_R16G16_UINT,
                    r16g16_sint = VK_FORMAT_R16G16_SINT,
                    r16g16_sfloat = VK_FORMAT_R16G16_SFLOAT,
                    r16g16b16_unorm = VK_FORMAT_R16G16B16_UNORM,
                    r16g16b16_snorm = VK_FORMAT_R16G16B16_SNORM,
                    r16g16b16_uscaled = VK_FORMAT_R16G16B16_USCALED,
                    r16g16b16_sscaled = VK_FORMAT_R16G16B16_SSCALED,
                    r16g16b16_uint = VK_FORMAT_R16G16B16_UINT,
                    r16g16b16_sint = VK_FORMAT_R16G16B16_SINT,
                    r16g16b16_sfloat = VK_FORMAT_R16G16B16_SFLOAT,
                    r16g16b16a16_unorm = VK_FORMAT_R16G16B16A16_UNORM,
                    r16g16b16a16_snorm = VK_FORMAT_R16G16B16A16_SNORM,
                    r16g16b16a16_uscaled = VK_FORMAT_R16G16B16A16_USCALED,
                    r16g16b16a16_sscaled = VK_FORMAT_R16G16B16A16_SSCALED,
                    r16g16b16a16_uint = VK_FORMAT_R16G16B16A16_UINT,
                    r16g16b16a16_sint = VK_FORMAT_R16G16B16A16_SINT,
                    r16g16b16a16_sfloat = VK_FORMAT_R16G16B16A16_SFLOAT,
                    r32_uint = VK_FORMAT_R32_UINT,
                    r32_sint = VK_FORMAT_R32_SINT,
                    r32_sfloat = VK_FORMAT_R32_SFLOAT,
                    r32g32_uint = VK_FORMAT_R32G32_UINT,
                    r32g32_sint = VK_FORMAT_R32G32_SINT,
                    r32g32_sfloat = VK_FORMAT_R32G32_SFLOAT,
                    r32g32b32_uint = VK_FORMAT_R32G32B32_UINT,
                    r32g32b32_sint = VK_FORMAT_R32G32B32_SINT,
                    r32g32b32_sfloat = VK_FORMAT_R32G32B32_SFLOAT,
                    r32g32b32a32_uint = VK_FORMAT_R32G32B32A32_UINT,
                    r32g32b32a32_sint = VK_FORMAT_R32G32B32A32_SINT,
                    r32g32b32a32_sfloat = VK_FORMAT_R32G32B32A32_SFLOAT,
                    r64_uint = VK_FORMAT_R64_UINT,
                    r64_sint = VK_FORMAT_R64_SINT,
                    r64_sfloat = VK_FORMAT_R64_SFLOAT,
                    r64g64_uint = VK_FORMAT_R64G64_UINT,
                    r64g64_sint = VK_FORMAT_R64G64_SINT,
                    r64g64_sfloat = VK_FORMAT_R64G64_SFLOAT,
                    r64g64b64_uint = VK_FORMAT_R64G64B64_UINT,
                    r64g64b64_sint = VK_FORMAT_R64G64B64_SINT,
                    r64g64b64_sfloat = VK_FORMAT_R64G64B64_SFLOAT,
                    r64g64b64a64_uint = VK_FORMAT_R64G64B64A64_UINT,
                    r64g64b64a64_sint = VK_FORMAT_R64G64B64A64_SINT,
                    r64g64b64a64_sfloat = VK_FORMAT_R64G64B64A64_SFLOAT,
                    b10g11r11_ufloat_pack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
                    e5b9g9r9_ufloat_pack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
                    d16_unorm = VK_FORMAT_D16_UNORM,
                    x8_d24_unorm_pack32 = VK_FORMAT_X8_D24_UNORM_PACK32,
                    d32_sfloat = VK_FORMAT_D32_SFLOAT,
                    s8_uint = VK_FORMAT_S8_UINT,
                    d16_unorm_s8_uint = VK_FORMAT_D16_UNORM_S8_UINT,
                    d24_unorm_s8_uint = VK_FORMAT_D24_UNORM_S8_UINT,
                    d32_sfloat_s8_uint = VK_FORMAT_D32_SFLOAT_S8_UINT,
                    bc1_rgb_unorm_block = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
                    bc1_rgb_srgb_block = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
                    bc1_rgba_unorm_block = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
                    bc1_rgba_srgb_block = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
                    bc2_unorm_block = VK_FORMAT_BC2_UNORM_BLOCK,
                    bc2_srgb_block = VK_FORMAT_BC2_SRGB_BLOCK,
                    bc3_unorm_block = VK_FORMAT_BC3_UNORM_BLOCK,
                    bc3_srgb_block = VK_FORMAT_BC3_SRGB_BLOCK,
                    bc4_unorm_block = VK_FORMAT_BC4_UNORM_BLOCK,
                    bc4_snorm_block = VK_FORMAT_BC4_SNORM_BLOCK,
                    bc5_unorm_block = VK_FORMAT_BC5_UNORM_BLOCK,
                    bc5_snorm_block = VK_FORMAT_BC5_SNORM_BLOCK,
                    bc6h_ufloat_block = VK_FORMAT_BC6H_UFLOAT_BLOCK,
                    bc6h_sfloat_block = VK_FORMAT_BC6H_SFLOAT_BLOCK,
                    bc7_unorm_block = VK_FORMAT_BC7_UNORM_BLOCK,
                    bc7_srgb_block = VK_FORMAT_BC7_SRGB_BLOCK,
                    etc2_r8g8b8_unorm_block = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
                    etc2_r8g8b8_srgb_block = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
                    etc2_r8g8b8a1_unorm_block = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
                    etc2_r8g8b8a1_srgb_block = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
                    etc2_r8g8b8a8_unorm_block = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
                    etc2_r8g8b8a8_srgb_block = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
                    eac_r11_unorm_block = VK_FORMAT_EAC_R11_UNORM_BLOCK,
                    eac_r11_snorm_block = VK_FORMAT_EAC_R11_SNORM_BLOCK,
                    eac_r11g11_unorm_block = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
                    eac_r11g11_snorm_block = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
                    astc_4x4_unorm_block = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
                    astc_4x4_srgb_block = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
                    astc_5x4_unorm_block = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
                    astc_5x4_srgb_block = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
                    astc_5x5_unorm_block = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
                    astc_5x5_srgb_block = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
                    astc_6x5_unorm_block = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
                    astc_6x5_srgb_block = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
                    astc_6x6_unorm_block = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
                    astc_6x6_srgb_block = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
                    astc_8x5_unorm_block = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
                    astc_8x5_srgb_block = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
                    astc_8x6_unorm_block = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
                    astc_8x6_srgb_block = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
                    astc_8x8_unorm_block = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
                    astc_8x8_srgb_block = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
                    astc_10x5_unorm_block = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
                    astc_10x5_srgb_block = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
                    astc_10x6_unorm_block = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
                    astc_10x6_srgb_block = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
                    astc_10x8_unorm_block = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
                    astc_10x8_srgb_block = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
                    astc_10x10_unorm_block = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
                    astc_10x10_srgb_block = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
                    astc_12x10_unorm_block = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
                    astc_12x10_srgb_block = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
                    astc_12x12_unorm_block = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
                    astc_12x12_srgb_block = VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
                    g8b8g8r8_422_unorm = VK_FORMAT_G8B8G8R8_422_UNORM,
                    b8g8r8g8_422_unorm = VK_FORMAT_B8G8R8G8_422_UNORM,
                    g8_b8_r8_3plane_420_unorm = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
                    g8_b8r8_2plane_420_unorm = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,
                    g8_b8_r8_3plane_422_unorm = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
                    g8_b8r8_2plane_422_unorm = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,
                    g8_b8_r8_3plane_444_unorm = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
                    r10x6_unorm_pack16 = VK_FORMAT_R10X6_UNORM_PACK16,
                    r10x6g10x6_unorm_2pack16 = VK_FORMAT_R10X6G10X6_UNORM_2PACK16,
                    r10x6g10x6b10x6a10x6_unorm_4pack16 = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
                    g10x6b10x6g10x6r10x6_422_unorm_4pack16 = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
                    b10x6g10x6r10x6g10x6_422_unorm_4pack16 = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
                    g10x6_b10x6_r10x6_3plane_420_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
                    g10x6_b10x6r10x6_2plane_420_unorm_3pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
                    g10x6_b10x6_r10x6_3plane_422_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
                    g10x6_b10x6r10x6_2plane_422_unorm_3pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
                    g10x6_b10x6_r10x6_3plane_444_unorm_3pack16 = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
                    r12x4_unorm_pack16 = VK_FORMAT_R12X4_UNORM_PACK16,
                    r12x4g12x4_unorm_2pack16 = VK_FORMAT_R12X4G12X4_UNORM_2PACK16,
                    r12x4g12x4b12x4a12x4_unorm_4pack16 = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
                    g12x4b12x4g12x4r12x4_422_unorm_4pack16 = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
                    b12x4g12x4r12x4g12x4_422_unorm_4pack16 = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
                    g12x4_b12x4_r12x4_3plane_420_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
                    g12x4_b12x4r12x4_2plane_420_unorm_3pack16 = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
                    g12x4_b12x4_r12x4_3plane_422_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
                    g12x4_b12x4r12x4_2plane_422_unorm_3pack16 = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
                    g12x4_b12x4_r12x4_3plane_444_unorm_3pack16 = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
                    g16b16g16r16_422_unorm = VK_FORMAT_G16B16G16R16_422_UNORM,
                    b16g16r16g16_422_unorm = VK_FORMAT_B16G16R16G16_422_UNORM,
                    g16_b16_r16_3plane_420_unorm = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
                    g16_b16r16_2plane_420_unorm = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,
                    g16_b16_r16_3plane_422_unorm = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
                    g16_b16r16_2plane_422_unorm = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,
                    g16_b16_r16_3plane_444_unorm = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
                    g8_b8r8_2plane_444_unorm = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM,
                    g10x6_b10x6r10x6_2plane_444_unorm_3pack16 = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16,
                    g12x4_b12x4r12x4_2plane_444_unorm_3pack16 = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16,
                    g16_b16r16_2plane_444_unorm = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM,
                    a4r4g4b4_unorm_pack16 = VK_FORMAT_A4R4G4B4_UNORM_PACK16,
                    a4b4g4r4_unorm_pack16 = VK_FORMAT_A4B4G4R4_UNORM_PACK16,
                    astc_4x4_sfloat_block = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK,
                    astc_5x4_sfloat_block = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK,
                    astc_5x5_sfloat_block = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK,
                    astc_6x5_sfloat_block = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK,
                    astc_6x6_sfloat_block = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK,
                    astc_8x5_sfloat_block = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK,
                    astc_8x6_sfloat_block = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK,
                    astc_8x8_sfloat_block = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK,
                    astc_10x5_sfloat_block = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK,
                    astc_10x6_sfloat_block = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK,
                    astc_10x8_sfloat_block = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK,
                    astc_10x10_sfloat_block = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK,
                    astc_12x10_sfloat_block = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK,
                    astc_12x12_sfloat_block = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK,
                    pvrtc1_2bpp_unorm_block_img = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
                    pvrtc1_4bpp_unorm_block_img = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
                    pvrtc2_2bpp_unorm_block_img = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
                    pvrtc2_4bpp_unorm_block_img = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
                    pvrtc1_2bpp_srgb_block_img = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
                    pvrtc1_4bpp_srgb_block_img = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
                    pvrtc2_2bpp_srgb_block_img = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
                    pvrtc2_4bpp_srgb_block_img = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,
                    r16g16_sfixed5_nv = VK_FORMAT_R16G16_SFIXED5_NV,
                    a1b5g5r5_unorm_pack16_khr = VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR,
                    a8_unorm_khr = VK_FORMAT_A8_UNORM_KHR,
                    astc_4x4_sfloat_block_ext = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT,
                    astc_5x4_sfloat_block_ext = VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT,
                    astc_5x5_sfloat_block_ext = VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT,
                    astc_6x5_sfloat_block_ext = VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT,
                    astc_6x6_sfloat_block_ext = VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT,
                    astc_8x5_sfloat_block_ext = VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT,
                    astc_8x6_sfloat_block_ext = VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT,
                    astc_8x8_sfloat_block_ext = VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT,
                    astc_10x5_sfloat_block_ext = VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT,
                    astc_10x6_sfloat_block_ext = VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT,
                    astc_10x8_sfloat_block_ext = VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT,
                    astc_10x10_sfloat_block_ext = VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT,
                    astc_12x10_sfloat_block_ext = VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT,
                    astc_12x12_sfloat_block_ext = VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT,
                    g8b8g8r8_422_unorm_khr = VK_FORMAT_G8B8G8R8_422_UNORM_KHR,
                    b8g8r8g8_422_unorm_khr = VK_FORMAT_B8G8R8G8_422_UNORM_KHR,
                    g8_b8_r8_3plane_420_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR,
                    g8_b8r8_2plane_420_unorm_khr = VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR,
                    g8_b8_r8_3plane_422_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR,
                    g8_b8r8_2plane_422_unorm_khr = VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR,
                    g8_b8_r8_3plane_444_unorm_khr = VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR,
                    r10x6_unorm_pack16_khr = VK_FORMAT_R10X6_UNORM_PACK16_KHR,
                    r10x6g10x6_unorm_2pack16_khr = VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR,
                    r10x6g10x6b10x6a10x6_unorm_4pack16_khr = VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR,
                    g10x6b10x6g10x6r10x6_422_unorm_4pack16_khr = VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR,
                    b10x6g10x6r10x6g10x6_422_unorm_4pack16_khr = VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR,
                    g10x6_b10x6_r10x6_3plane_420_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR,
                    g10x6_b10x6r10x6_2plane_420_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR,
                    g10x6_b10x6_r10x6_3plane_422_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR,
                    g10x6_b10x6r10x6_2plane_422_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR,
                    g10x6_b10x6_r10x6_3plane_444_unorm_3pack16_khr = VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR,
                    r12x4_unorm_pack16_khr = VK_FORMAT_R12X4_UNORM_PACK16_KHR,
                    r12x4g12x4_unorm_2pack16_khr = VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR,
                    r12x4g12x4b12x4a12x4_unorm_4pack16_khr = VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR,
                    g12x4b12x4g12x4r12x4_422_unorm_4pack16_khr = VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR,
                    b12x4g12x4r12x4g12x4_422_unorm_4pack16_khr = VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR,
                    g12x4_b12x4_r12x4_3plane_420_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR,
                    g12x4_b12x4r12x4_2plane_420_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR,
                    g12x4_b12x4_r12x4_3plane_422_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR,
                    g12x4_b12x4r12x4_2plane_422_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR,
                    g12x4_b12x4_r12x4_3plane_444_unorm_3pack16_khr = VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR,
                    g16b16g16r16_422_unorm_khr = VK_FORMAT_G16B16G16R16_422_UNORM_KHR,
                    b16g16r16g16_422_unorm_khr = VK_FORMAT_B16G16R16G16_422_UNORM_KHR,
                    g16_b16_r16_3plane_420_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR,
                    g16_b16r16_2plane_420_unorm_khr = VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR,
                    g16_b16_r16_3plane_422_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR,
                    g16_b16r16_2plane_422_unorm_khr = VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR,
                    g16_b16_r16_3plane_444_unorm_khr = VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR,
                    g8_b8r8_2plane_444_unorm_ext = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT,
                    g10x6_b10x6r10x6_2plane_444_unorm_3pack16_ext = VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT,
                    g12x4_b12x4r12x4_2plane_444_unorm_3pack16_ext = VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT,
                    g16_b16r16_2plane_444_unorm_ext = VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT,
                    a4r4g4b4_unorm_pack16_ext = VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT,
                    a4b4g4r4_unorm_pack16_ext = VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT,
                    r16g16_s10_5_nv = VK_FORMAT_R16G16_S10_5_NV,
                };
                enum class Color_space : std::uint64_t
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

                Pixel pixel;
                Color_space color_space;
                Mode mode;
                std::size_t images_count;
            };

            using enum Kind;

            Kind kind;
            Feature features;

            std::span<const Queue_family> queue_families;
            std::span<const char*> extensions;
            std::span<const Limit> limits;
            Swap_chain swap_chain;
        };
        struct Display
        {
            enum class Kind : std::uint64_t
            {
                primary = 0x1u,
                secondary = 0x2u
            };

            using enum Kind;

            Kind kind;
            common::Extent<std::uint32_t> logical_size;
            common::Extent<std::uint32_t> physical_size;
            std::uint8_t bits_per_pixel;
        };

        std::vector<const device::GPU*>
        operator()(std::span<const device::GPU*> devices_a, const canvas::Windowed* p_window, const filter::GPU& requirements_a);
        std::vector<const device::Display*> operator()(std::span<const device::Display*> devices_a, const filter::Display& requirements_a);
    } filter;
};

constexpr device::GPU::Properties::Kind operator|(device::GPU::Properties::Kind left_a, device::GPU::Properties::Kind right_a)
{
    return static_cast<device::GPU::Properties::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr device::GPU::Properties::Kind operator&(device::GPU::Properties::Kind left_a, device::GPU::Properties::Kind right_a)
{
    return static_cast<device::GPU::Properties::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}

constexpr device::GPU::Properties::Feature operator|(device::GPU::Properties::Feature left_a, device::GPU::Properties::Feature right_a)
{
    return static_cast<device::GPU::Properties::Feature>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr device::GPU::Properties::Feature operator&(device::GPU::Properties::Feature left_a, device::GPU::Properties::Feature right_a)
{
    return static_cast<device::GPU::Properties::Feature>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}

constexpr device::GPU::Properties::Queue_family::Kind operator|(device::GPU::Properties::Queue_family::Kind left_a,
                                                                device::GPU::Properties::Queue_family::Kind right_a)
{
    return static_cast<device::GPU::Properties::Queue_family::Kind>(static_cast<std::uint64_t>(left_a) |
                                                                    static_cast<std::uint64_t>(right_a));
}
constexpr device::GPU::Properties::Queue_family::Kind operator&(device::GPU::Properties::Queue_family::Kind left_a,
                                                                device::GPU::Properties::Queue_family::Kind right_a)
{
    return static_cast<device::GPU::Properties::Queue_family::Kind>(static_cast<std::uint64_t>(left_a) &
                                                                    static_cast<std::uint64_t>(right_a));
}

constexpr device::GPU::Properties::Limits::Sample_count operator|(device::GPU::Properties::Limits::Sample_count left_a,
                                                                  device::GPU::Properties::Limits::Sample_count right_a)
{
    return static_cast<device::GPU::Properties::Limits::Sample_count>(static_cast<std::uint64_t>(left_a) |
                                                                      static_cast<std::uint64_t>(right_a));
}
constexpr device::GPU::Properties::Limits::Sample_count operator&(device::GPU::Properties::Limits::Sample_count left_a,
                                                                  device::GPU::Properties::Limits::Sample_count right_a)
{
    return static_cast<device::GPU::Properties::Limits::Sample_count>(static_cast<std::uint64_t>(left_a) &
                                                                      static_cast<std::uint64_t>(right_a));
}

constexpr device::Display::Properties::Kind operator|(device::Display::Properties::Kind left_a, device::Display::Properties::Kind right_a)
{
    return static_cast<device::Display::Properties::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr device::Display::Properties::Kind operator&(device::Display::Properties::Kind left_a, device::Display::Properties::Kind right_a)
{
    return static_cast<device::Display::Properties::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}

constexpr device::filter::Display::Kind operator|(device::filter::Display::Kind left_a, device::filter::Display::Kind right_a)
{
    return static_cast<device::filter::Display::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr device::filter::Display::Kind operator&(device::filter::Display::Kind left_a, device::filter::Display::Kind right_a)
{
    return static_cast<device::filter::Display::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}

constexpr device::filter::GPU::Queue_family::Kind operator|(device::filter::GPU::Queue_family::Kind left_a,
                                                            device::filter::GPU::Queue_family::Kind right_a)
{
    return static_cast<device::filter::GPU::Queue_family::Kind>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr device::filter::GPU::Queue_family::Kind operator&(device::filter::GPU::Queue_family::Kind left_a,
                                                            device::filter::GPU::Queue_family::Kind right_a)
{
    return static_cast<device::filter::GPU::Queue_family::Kind>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
} // namespace lxf