// this
#include <lx/gpu/Pipeline.hpp>

namespace lx::gpu {
using namespace lx::containers;

Pipeline<pipeline::graphics>::Pipeline(VkDevice vk_device_a, const Properties& properties_a)
{
    Vector<VkVertexInputBindingDescription> vk_vertex_input_binding_descriptions(properties_a.vertex_input.bindings.size());
    Vector<VkVertexInputAttributeDescription> vk_vertex_input_attribute_descriptions(properties_a.vertex_input.attributes.size());

    for (const auto& vertex_binding : properties_a.vertex_input.bindings)
    {
        vk_vertex_input_binding_descriptions.emplace_back(
            vertex_binding.binding, vertex_binding.stride, static_cast<VkVertexInputRate>(vertex_binding.rate));
    }
    for (const auto& vertex_attribute : properties_a.vertex_input.attributes)
    {
        vk_vertex_input_attribute_descriptions.emplace_back(
            vertex_attribute.location, vertex_attribute.binding, static_cast<VkFormat>(vertex_attribute.format), vertex_attribute.offset);
    }

    VkPipelineVertexInputStateCreateInfo vk_pipeline_vertex_input_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .vertexBindingDescriptionCount = static_cast<std::uint32_t>(properties_a.vertex_input.bindings.size()),
        .pVertexBindingDescriptions = vk_vertex_input_binding_descriptions.get_buffer(),
        .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(properties_a.vertex_input.attributes.size()),
        .pVertexAttributeDescriptions = vk_vertex_input_attribute_descriptions.get_buffer()
    };

    VkPipelineInputAssemblyStateCreateInfo vk_pipeline_input_assembly_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .topology = static_cast<VkPrimitiveTopology>(properties_a.primitive.topology),
        .primitiveRestartEnable = true == properties_a.primitive.primitive_restart ? VK_TRUE : VK_FALSE
    };

    VkPipelineColorBlendStateCreateInfo vk_pipeline_color_blend_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, .pNext = nullptr, .flags = 0x0u, .logicOpEnable = VK_TRUE,
        //.logicOp
    };

    VkGraphicsPipelineCreateInfo vk_graphics_pipeline_create_info { .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                                                                    .pNext = nullptr,
                                                                    .flags = 0u,
                                                                    .pVertexInputState = &vk_pipeline_vertex_input_state_create_info,
                                                                    .pInputAssemblyState = &vk_pipeline_input_assembly_state_create_info,
                                                                    .pColorBlendState = &vk_pipeline_color_blend_state_create_info };
}

void Pipeline<pipeline::graphics>::destroy(VkDevice vk_device_a) {}
} // namespace lx::gpu