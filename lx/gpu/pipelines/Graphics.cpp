// this
#include <lx/gpu/pipelines/Graphics.hpp>

namespace lx::gpu::pipelines {
Graphics::Graphics(const Properties& properties_a)
    : vk_pipeline(VK_NULL_HANDLE)
    , vk_pipeline_layout(VK_NULL_HANDLE)
{
    VkPipelineVertexInputStateCreateInfo vk_pipeline_vertex_input_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, .pNext = nullptr, .flags = 0u
    };

    VkPipelineInputAssemblyStateCreateInfo vk_pipeline_input_assembly_state_create_info {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .topology = static_cast<VkPrimitiveTopology>(properties_a.primitive.topology),
        .primitiveRestartEnable = true == properties_a.primitive.primitive_restart ? VK_TRUE : VK_FALSE
    };

    VkGraphicsPipelineCreateInfo vk_graphics_pipeline_create_info { .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                                                                    .pNext = nullptr,
                                                                    .flags = 0u,
                                                                    .pInputAssemblyState = &vk_pipeline_input_assembly_state_create_info };
}
} // namespace lx::gpu::pipelines