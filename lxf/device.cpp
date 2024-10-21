/*
 *	Name: device.cpp
 *  Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/device.hpp>

// std
#include <limits>

// lxf
#include <lxf/loader/vulkan.hpp>

namespace lxf {
using namespace common;

template<typename Value_x> bool check_limit(const device::Filter<device::GPU>::Requirements::Limit req_limit_a, Value_x value_a)
{
    Value_x l = *(std::bit_cast<Value_x*>(&(req_limit_a.value.data[0])));

    switch (req_limit_a.value.operation)
    {
        case device::Filter<device::GPU>::Requirements::Limit::Value::Operation::greater: {
            return value_a > l;
        }
        break;
        case device::Filter<device::GPU>::Requirements::Limit::Value::Operation::greater_or_equal: {
            return value_a >= l;
        }
        break;
        case device::Filter<device::GPU>::Requirements::Limit::Value::Operation::less: {
            return value_a < l;
        }
        break;
        case device::Filter<device::GPU>::Requirements::Limit::Value::Operation::less_or_equal: {
            return value_a <= l;
        }
        break;
        case device::Filter<device::GPU>::Requirements::Limit::Value::Operation::equals: {
            return value_a == l;
        }
        break;
    }

    return false;
};

device::GPU::GPU(VkPhysicalDevice vk_physical_device_a,
                 bool is_primary_a,
                 std::span<VkQueueFamilyProperties> queues_a,
                 std::string_view name_a,
                 const std::vector<std::string_view>& extension_names_a)
    : vk_physical_device(vk_physical_device_a)
    , info_idx(std::numeric_limits<std::size_t>::max())
{
    auto calculate_extensions_buffer_size = [&extension_names_a]() -> std::size_t {
        std::size_t ret = 0u;

        for (std::string_view extension_name : extension_names_a)
        {
            ret += extension_name.size() + 1u;
        }

        return ret;
    };

    const std::size_t name_buffer_size_bytes = name_a.size() + 1u;
    const std::size_t queue_families_buffer_size_bytes = queues_a.size() * sizeof(Properties::Queue_family);
    const std::size_t extensions_buffer_size_bytes = calculate_extensions_buffer_size();
    const std::size_t flags_buffer_size_bytes = 8u;

    Info gpu_info {
        .layout { .name_buffer { .offset_bytes = 0u, .size_bytes = name_buffer_size_bytes },
                  .queue_families_buffer { .offset_bytes = name_buffer_size_bytes, .size_bytes = queue_families_buffer_size_bytes },
                  .extensions_buffer { .offset_bytes = name_buffer_size_bytes + queue_families_buffer_size_bytes,
                                       .size_bytes = extensions_buffer_size_bytes },
                  .flags_buffer { .offset_bytes = name_buffer_size_bytes + queue_families_buffer_size_bytes + extensions_buffer_size_bytes,
                                  .size_bytes = flags_buffer_size_bytes } },
    };

    gpu_info.buffer =
        std::make_unique<std::byte[]>(gpu_info.layout.name_buffer.size_bytes + gpu_info.layout.queue_families_buffer.size_bytes +
                                      gpu_info.layout.extensions_buffer.size_bytes + gpu_info.layout.flags_buffer.size_bytes /*flags*/);

    std::byte* p_current =
        std::bit_cast<std::byte*>(std::copy(name_a.begin(), name_a.end(), std::bit_cast<char*>(gpu_info.buffer.get()))) + 1u;

    std::size_t qf_index = 0;
    for (const VkQueueFamilyProperties& vk_queue_family_properties : queues_a)
    {
        Properties::Queue_family q { .kind = static_cast<Properties::Queue_family::Kind>(vk_queue_family_properties.queueFlags),
                                     .count = vk_queue_family_properties.queueCount,
                                     .index = qf_index++ };
        p_current = std::copy(std::bit_cast<std::byte*>(&q), std::bit_cast<std::byte*>(&q) + sizeof(q), p_current);
    }

    for (std::string_view extension_name : extension_names_a)
    {
        p_current =
            std::bit_cast<std::byte*>(std::copy(extension_name.begin(), extension_name.end(), std::bit_cast<char*>(p_current))) + 1u;
    }

    if (true == is_primary_a)
    {
        std::uint64_t* p_flags = std::bit_cast<std::uint64_t*>(gpu_info.buffer.get() + gpu_info.layout.flags_buffer.offset_bytes);
        bit::set(p_flags, 1u);
    }

    gpu_info_buffer.push_back(std::move(gpu_info));
    this->info_idx = gpu_info_buffer.size() - 1u;
}

device::GPU::Properties device::GPU::get_properties() const
{
    VkPhysicalDeviceProperties vk_physical_device_properties;
    VkPhysicalDeviceFeatures vk_physical_device_features;

    vkGetPhysicalDeviceProperties(this->vk_physical_device, &vk_physical_device_properties);
    vkGetPhysicalDeviceFeatures(this->vk_physical_device, &vk_physical_device_features);

    std::vector<const char*> extensions;

    const char* p_extensions_current = std::bit_cast<const char*>(gpu_info_buffer[this->info_idx].buffer.get() +
                                                                  gpu_info_buffer[this->info_idx].layout.extensions_buffer.offset_bytes);
    const char* p_extensions_end = std::bit_cast<const char*>(gpu_info_buffer[this->info_idx].buffer.get() +
                                                              gpu_info_buffer[this->info_idx].layout.extensions_buffer.offset_bytes +
                                                              gpu_info_buffer[this->info_idx].layout.extensions_buffer.size_bytes);

    while (p_extensions_current != p_extensions_end)
    {
        extensions.push_back(p_extensions_current);
        p_extensions_current += std::strlen(p_extensions_current) + 1u;
    }

    const std::uint64_t* p_flags = std::bit_cast<std::uint64_t*>(gpu_info_buffer[this->info_idx].buffer.get() +
                                                                 gpu_info_buffer[this->info_idx].layout.flags_buffer.offset_bytes +
                                                                 gpu_info_buffer[this->info_idx].layout.flags_buffer.size_bytes);

    return { .kind = this->from(vk_physical_device_properties.deviceType) |
                     (true == bit::is(*p_flags, 1u) ? Properties::Kind::primary : Properties::Kind { 0x0 }),
             .features = this->from(vk_physical_device_features),
             .limits = this->from(vk_physical_device_properties.limits),
             .queue_families = gpu_info_buffer[this->info_idx].get_queue_families(),
             .extensions = extensions,
             .p_name = gpu_info_buffer[this->info_idx].get_name() };
}

std::vector<const device::GPU*> device::Filter<device::GPU>::operator()(std::span<GPU> gpus_a, const Requirements& requirements_a)
{
    std::vector<const GPU*> ret;
    ret.reserve(gpus_a.size());

    for (const GPU& gpu : gpus_a)
    {
        const GPU::Properties properties = gpu.get_properties();

        if (true == common::bit::is_any(requirements_a.kind, properties.kind) &&
            true == common::bit::flag::is(properties.features, requirements_a.features))
        {
            bool qf_compatible = true;

            std::vector<GPU::Properties::Queue_family> qf_temp { properties.queue_families.begin(), properties.queue_families.end() };

            for (std::size_t qf_req_index = 0; qf_req_index < requirements_a.queue_families.size() && true == qf_compatible; qf_req_index++)
            {
                qf_compatible = false == qf_temp.empty();

                if (true == qf_compatible)
                {
                    const Requirements::Queue_family qf_req = requirements_a.queue_families[qf_req_index];
                    auto found_itr =
                        std::find_if(qf_temp.begin(), qf_temp.end(), [&qf_req](const GPU::Properties::Queue_family& qf_a) -> bool {
                            return qf_a.count >= qf_req.count && true == common::bit::flag::is(qf_a.kind, qf_req.kind);
                        });

                    qf_compatible = found_itr != qf_temp.end();

                    if (true == qf_compatible)
                    {
                        qf_temp.erase(found_itr);
                    }
                }
            }

            if (true == qf_compatible)
            {
                if ((false == requirements_a.extensions.empty() && false == properties.extensions.empty()) ||
                    (true == requirements_a.extensions.empty() && true == properties.extensions.empty()) ||
                    (true == requirements_a.extensions.empty() && false == properties.extensions.empty()))
                {
                    // chceck extensions
                    bool extension_compatible = true;
                    for (std::size_t extension_req_index = 0u;
                         extension_req_index < requirements_a.extensions.size() && true == extension_compatible;
                         extension_req_index++)
                    {
                        auto found_itr =
                            std::find_if(properties.extensions.begin(), properties.extensions.end(), [&](const char* p_name_a) {
                                return 0 == strncmp(requirements_a.extensions[extension_req_index], p_name_a, 128u);
                            });

                        extension_compatible = properties.extensions.end() != found_itr;
                    }

                    if (true == extension_compatible)
                    {
                        // check limits
                        bool limit_compatible = true;
                        for (std::size_t limit_req_index = 0u; limit_req_index < requirements_a.limits.size() && true == limit_compatible;
                             limit_req_index++)
                        {
                            const Requirements::Limit limit = requirements_a.limits[limit_req_index];

                            switch (limit.kind)
                            {
                                case Requirements::Limit::Kind::max_image_dimension_1d: {
                                    limit_compatible = check_limit(limit, properties.limits.max_image_dimension_1d);
                                }
                                break;
                                case Requirements::Limit::Kind::max_image_dimension_2d: {
                                    limit_compatible = check_limit(limit, properties.limits.max_image_dimension_2d);
                                }
                                break;
                                case Requirements::Limit::Kind::max_image_dimension_3d: {
                                    limit_compatible = check_limit(limit, properties.limits.max_image_dimension_3d);
                                }
                                break;
                                case Requirements::Limit::Kind::max_image_dimensioncube: {
                                    limit_compatible = check_limit(limit, properties.limits.max_image_dimensioncube);
                                }
                                break;
                                case Requirements::Limit::Kind::max_image_arraylayers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_image_arraylayers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_texel_buffer_elements: {
                                    limit_compatible = check_limit(limit, properties.limits.max_texel_buffer_elements);
                                }
                                break;
                                case Requirements::Limit::Kind::max_uniform_buffer_range: {
                                    limit_compatible = check_limit(limit, properties.limits.max_uniform_buffer_range);
                                }
                                break;
                                case Requirements::Limit::Kind::max_storage_buffer_range: {
                                    limit_compatible = check_limit(limit, properties.limits.max_storage_buffer_range);
                                }
                                break;
                                case Requirements::Limit::Kind::max_push_constants_size: {
                                    limit_compatible = check_limit(limit, properties.limits.max_push_constants_size);
                                }
                                break;
                                case Requirements::Limit::Kind::max_memory_allocation_count: {
                                    limit_compatible = check_limit(limit, properties.limits.max_memory_allocation_count);
                                }
                                break;
                                case Requirements::Limit::Kind::max_sampler_allocation_count: {
                                    limit_compatible = check_limit(limit, properties.limits.max_sampler_allocation_count);
                                }
                                break;
                                case Requirements::Limit::Kind::buffer_image_granularity: {
                                    limit_compatible = check_limit(limit, properties.limits.buffer_image_granularity);
                                }
                                break;
                                case Requirements::Limit::Kind::sparse_address_space_size: {
                                    limit_compatible = check_limit(limit, properties.limits.sparse_address_space_size);
                                }
                                break;
                                case Requirements::Limit::Kind::max_bound_descriptor_sets: {
                                    limit_compatible = check_limit(limit, properties.limits.max_bound_descriptor_sets);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_descriptor_samplers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_descriptor_samplers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_descriptor_uniform_buffers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_descriptor_uniform_buffers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_descriptor_storage_buffers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_descriptor_storage_buffers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_descriptor_sampled_images: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_descriptor_sampled_images);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_descriptor_storage_images: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_descriptor_storage_images);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_descriptor_input_attachments: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_descriptor_input_attachments);
                                }
                                break;
                                case Requirements::Limit::Kind::max_per_stage_resources: {
                                    limit_compatible = check_limit(limit, properties.limits.max_per_stage_resources);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_samplers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_samplers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_uniform_buffers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_uniform_buffers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_uniform_buffers_dynamic: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_uniform_buffers_dynamic);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_storage_buffers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_storage_buffers);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_storage_buffers_dynamic: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_storage_buffers_dynamic);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_sampled_images: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_sampled_images);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_storage_images: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_storage_images);
                                }
                                break;
                                case Requirements::Limit::Kind::max_descriptor_set_input_attachments: {
                                    limit_compatible = check_limit(limit, properties.limits.max_descriptor_set_input_attachments);
                                }
                                break;
                                case Requirements::Limit::Kind::max_vertex_input_attributes: {
                                    limit_compatible = check_limit(limit, properties.limits.max_vertex_input_attributes);
                                }
                                break;
                                case Requirements::Limit::Kind::max_vertex_input_bindings: {
                                    limit_compatible = check_limit(limit, properties.limits.max_vertex_input_bindings);
                                }
                                break;
                                case Requirements::Limit::Kind::max_vertex_input_attribute_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.max_vertex_input_attribute_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::max_vertex_input_binding_stride: {
                                    limit_compatible = check_limit(limit, properties.limits.max_vertex_input_binding_stride);
                                }
                                break;
                                case Requirements::Limit::Kind::max_vertex_output_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_vertex_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_generationlevel: {
                                    limit_compatible = check_limit(limit, properties.limits.max_tessellation_generationlevel);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_patchsize: {
                                    limit_compatible = check_limit(limit, properties.limits.max_tessellation_patchsize);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_control_per_vertex_input_components: {
                                    limit_compatible =
                                        check_limit(limit, properties.limits.max_tessellation_control_per_vertex_input_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_control_per_vertex_output_components: {
                                    limit_compatible =
                                        check_limit(limit, properties.limits.max_tessellation_control_per_vertex_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_control_per_patch_output_components: {
                                    limit_compatible =
                                        check_limit(limit, properties.limits.max_tessellation_control_per_patch_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_control_total_output_components: {
                                    limit_compatible =
                                        check_limit(limit, properties.limits.max_tessellation_control_total_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_evaluation_input_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_tessellation_evaluation_input_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_tessellation_evaluation_output_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_tessellation_evaluation_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_geometry_shader_invocations: {
                                    limit_compatible = check_limit(limit, properties.limits.max_geometry_shader_invocations);
                                }
                                break;
                                case Requirements::Limit::Kind::max_geometry_input_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_geometry_input_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_geometry_output_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_geometry_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_geometry_output_vertices: {
                                    limit_compatible = check_limit(limit, properties.limits.max_geometry_output_vertices);
                                }
                                break;
                                case Requirements::Limit::Kind::max_geometry_total_output_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_geometry_total_output_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_fragment_input_components: {
                                    limit_compatible = check_limit(limit, properties.limits.max_fragment_input_components);
                                }
                                break;
                                case Requirements::Limit::Kind::max_fragment_output_attachments: {
                                    limit_compatible = check_limit(limit, properties.limits.max_fragment_output_attachments);
                                }
                                break;
                                case Requirements::Limit::Kind::max_fragment_dual_src_attachments: {
                                    limit_compatible = check_limit(limit, properties.limits.max_fragment_dual_src_attachments);
                                }
                                break;
                                case Requirements::Limit::Kind::max_fragment_combined_output_resources: {
                                    limit_compatible = check_limit(limit, properties.limits.max_fragment_combined_output_resources);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_shared_memory_size: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_shared_memory_size);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_count_0: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_count[0]);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_count_1: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_count[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_count_2: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_count[2]);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_invocations: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_invocations);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_size_0: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_size[0]);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_size_1: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_size[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::max_compute_work_group_size_2: {
                                    limit_compatible = check_limit(limit, properties.limits.max_compute_work_group_size[2]);
                                }
                                break;
                                case Requirements::Limit::Kind::sub_pixel_precision_bits: {
                                    limit_compatible = check_limit(limit, properties.limits.sub_pixel_precision_bits);
                                }
                                break;
                                case Requirements::Limit::Kind::sub_texel_precision_bits: {
                                    limit_compatible = check_limit(limit, properties.limits.sub_texel_precision_bits);
                                }
                                break;
                                case Requirements::Limit::Kind::mipmap_precision_bits: {
                                    limit_compatible = check_limit(limit, properties.limits.mipmap_precision_bits);
                                }
                                break;
                                case Requirements::Limit::Kind::max_draw_indexed_index_value: {
                                    limit_compatible = check_limit(limit, properties.limits.max_draw_indexed_index_value);
                                }
                                break;
                                case Requirements::Limit::Kind::max_draw_indirect_count: {
                                    limit_compatible = check_limit(limit, properties.limits.max_draw_indirect_count);
                                }
                                break;
                                case Requirements::Limit::Kind::max_sampler_lod_bias: {
                                    limit_compatible = check_limit(limit, properties.limits.max_sampler_lod_bias);
                                }
                                break;
                                case Requirements::Limit::Kind::max_sampler_anisotropy: {
                                    limit_compatible = check_limit(limit, properties.limits.max_sampler_anisotropy);
                                }
                                break;
                                case Requirements::Limit::Kind::max_viewports: {
                                    limit_compatible = check_limit(limit, properties.limits.max_viewports);
                                }
                                break;
                                case Requirements::Limit::Kind::max_viewport_dimensions_0: {
                                    limit_compatible = check_limit(limit, properties.limits.max_viewport_dimensions[0]);
                                }
                                break;
                                case Requirements::Limit::Kind::max_viewport_dimensions_1: {
                                    limit_compatible = check_limit(limit, properties.limits.max_viewport_dimensions[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::viewport_bounds_range_0: {
                                    limit_compatible = check_limit(limit, properties.limits.viewport_bounds_range[0]);
                                }
                                break;
                                case Requirements::Limit::Kind::viewport_bounds_range_1: {
                                    limit_compatible = check_limit(limit, properties.limits.viewport_bounds_range[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::viewport_sub_pixel_bits: {
                                    limit_compatible = check_limit(limit, properties.limits.viewport_sub_pixel_bits);
                                }
                                break;
                                case Requirements::Limit::Kind::min_memory_map_alignment: {
                                    limit_compatible = check_limit(limit, properties.limits.min_memory_map_alignment);
                                }
                                break;
                                case Requirements::Limit::Kind::min_texel_buffer_offset_alignment: {
                                    limit_compatible = check_limit(limit, properties.limits.min_texel_buffer_offset_alignment);
                                }
                                break;
                                case Requirements::Limit::Kind::min_uniform_buffer_offset_alignment: {
                                    limit_compatible = check_limit(limit, properties.limits.min_uniform_buffer_offset_alignment);
                                }
                                break;
                                case Requirements::Limit::Kind::min_storage_buffer_offset_alignment: {
                                    limit_compatible = check_limit(limit, properties.limits.min_storage_buffer_offset_alignment);
                                }
                                break;
                                case Requirements::Limit::Kind::min_texel_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.min_texel_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::max_texel_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.max_texel_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::min_texel_gather_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.min_texel_gather_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::max_texel_gather_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.max_texel_gather_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::min_interpolation_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.min_interpolation_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::max_interpolation_offset: {
                                    limit_compatible = check_limit(limit, properties.limits.max_interpolation_offset);
                                }
                                break;
                                case Requirements::Limit::Kind::sub_pixel_interpolation_offset_bits: {
                                    limit_compatible = check_limit(limit, properties.limits.sub_pixel_interpolation_offset_bits);
                                }
                                break;
                                case Requirements::Limit::Kind::max_framebuffer_width: {
                                    limit_compatible = check_limit(limit, properties.limits.max_framebuffer_width);
                                }
                                break;
                                case Requirements::Limit::Kind::max_framebuffer_height: {
                                    limit_compatible = check_limit(limit, properties.limits.max_framebuffer_height);
                                }
                                break;
                                case Requirements::Limit::Kind::max_framebuffer_layers: {
                                    limit_compatible = check_limit(limit, properties.limits.max_framebuffer_layers);
                                }
                                break;
                                case Requirements::Limit::Kind::framebuffer_color_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.framebuffer_color_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::framebuffer_depth_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.framebuffer_stencil_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::framebuffer_stencil_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.framebuffer_stencil_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::framebuffer_no_attachments_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.framebuffer_no_attachments_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::max_color_attachments: {
                                    limit_compatible = check_limit(limit, properties.limits.max_color_attachments);
                                }
                                break;
                                case Requirements::Limit::Kind::sampled_image_color_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.sampled_image_color_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::sampled_image_integer_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.sampled_image_integer_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::sampled_image_depth_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.sampled_image_depth_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::sampled_image_stencil_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.sampled_image_stencil_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::storage_image_sample_counts: {
                                    limit_compatible = check_limit(limit, properties.limits.storage_image_sample_counts);
                                }
                                break;
                                case Requirements::Limit::Kind::max_sample_mask_words: {
                                    limit_compatible = check_limit(limit, properties.limits.max_sample_mask_words);
                                }
                                break;
                                case Requirements::Limit::Kind::timestamp_compute_and_graphics: {
                                    limit_compatible = check_limit(limit, properties.limits.timestamp_compute_and_graphics);
                                }
                                break;
                                case Requirements::Limit::Kind::timestamp_period: {
                                    limit_compatible = check_limit(limit, properties.limits.timestamp_period);
                                }
                                break;
                                case Requirements::Limit::Kind::max_clip_distances: {
                                    limit_compatible = check_limit(limit, properties.limits.max_clip_distances);
                                }
                                break;
                                case Requirements::Limit::Kind::max_cull_distances: {
                                    limit_compatible = check_limit(limit, properties.limits.max_cull_distances);
                                }
                                break;
                                case Requirements::Limit::Kind::max_combined_clip_and_cull_distances: {
                                    limit_compatible = check_limit(limit, properties.limits.max_combined_clip_and_cull_distances);
                                }
                                break;
                                case Requirements::Limit::Kind::discrete_queue_priorities: {
                                    limit_compatible = check_limit(limit, properties.limits.point_size_range[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::point_size_range_0: {
                                    limit_compatible = check_limit(limit, properties.limits.point_size_range[0]);
                                }
                                break;
                                case Requirements::Limit::Kind::point_size_range_1: {
                                    limit_compatible = check_limit(limit, properties.limits.point_size_range[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::line_width_hrange_0: {
                                    limit_compatible = check_limit(limit, properties.limits.line_width_hrange[0]);
                                }
                                break;
                                case Requirements::Limit::Kind::line_width_hrange_1: {
                                    limit_compatible = check_limit(limit, properties.limits.line_width_hrange[1]);
                                }
                                break;
                                case Requirements::Limit::Kind::point_size_granularity: {
                                    limit_compatible = check_limit(limit, properties.limits.point_size_granularity);
                                }
                                break;
                                case Requirements::Limit::Kind::line_width_granularity: {
                                    limit_compatible = check_limit(limit, properties.limits.line_width_granularity);
                                }
                                break;
                                case Requirements::Limit::Kind::strict_lines: {
                                    limit_compatible = check_limit(limit, properties.limits.strict_lines);
                                }
                                break;
                                case Requirements::Limit::Kind::standard_sample_locations: {
                                    limit_compatible = check_limit(limit, properties.limits.standard_sample_locations);
                                }
                                break;
                                case Requirements::Limit::Kind::optimal_buffer_copy_offset_alignment: {
                                    limit_compatible = check_limit(limit, properties.limits.optimal_buffer_copy_offset_alignment);
                                }
                                break;
                                case Requirements::Limit::Kind::optimal_buffer_copy_row_pitch_alignment: {
                                    limit_compatible = check_limit(limit, properties.limits.optimal_buffer_copy_row_pitch_alignment);
                                }
                                break;
                                case Requirements::Limit::Kind::non_coherent_atom_size: {
                                    limit_compatible = check_limit(limit, properties.limits.non_coherent_atom_size);
                                }
                                break;
                            }
                        }

                        if (true == limit_compatible)
                        {
                            ret.push_back(&gpu);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

} // namespace lxf