#pragma once

// lx
#include <lx/common/non_copyable.hpp>
#include <lx/gpu/loader/vulkan.hpp>

namespace lx::gpu::pipelines {
class Graphics : private lx::common::non_copyable
{
public:
    /*
    typedef struct VkPipelineShaderStageCreateInfo {
        VkShaderStageFlagBits               stage;
        VkShaderModule                      module;
        const char*                         pName;
        const VkSpecializationInfo*         pSpecializationInfo;
    } VkPipelineShaderStageCreateInfo;
    typedef struct VkPipelineVertexInputStateCreateInfo {
        uint32_t                                    vertexBindingDescriptionCount;
        const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
        uint32_t                                    vertexAttributeDescriptionCount;
        const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
    } VkPipelineVertexInputStateCreateInfo;
    typedef struct VkPipelineTessellationStateCreateInfo {
        uint32_t                                  patchControlPoints;
    } VkPipelineTessellationStateCreateInfo;



    typedef struct VkPipelineViewportStateCreateInfo {
        uint32_t                              viewportCount;
        const VkViewport*                     pViewports;
        uint32_t                              scissorCount;
        const VkRect2D*                       pScissors;
    } VkPipelineViewportStateCreateInfo;

    typedef struct VkPipelineRasterizationStateCreateInfo {
        VkBool32                                   rasterizerDiscardEnable;
        float                                      lineWidth;
    } VkPipelineRasterizationStateCreateInfo;

    typedef struct VkPipelineMultisampleStateCreateInfo {
        const VkSampleMask*                      pSampleMask;
    } VkPipelineMultisampleStateCreateInfo;

    typedef struct VkPipelineColorBlendStateCreateInfo {
        VkBool32                                      logicOpEnable;
        VkLogicOp                                     logicOp;
        uint32_t                                      attachmentCount;
        const VkPipelineColorBlendAttachmentState*    pAttachments;
        float                                         blendConstants[4];
    } VkPipelineColorBlendStateCreateInfo;

    typedef struct VkPipelineDynamicStateCreateInfo {
        uint32_t                             dynamicStateCount;
        const VkDynamicState*                pDynamicStates;
    } VkPipelineDynamicStateCreateInfo;

    */

    struct PrimitiveProperties
    {
        enum class PolygonMode : std::uint32_t
        {
            fill = VK_POLYGON_MODE_FILL,
            line = VK_POLYGON_MODE_LINE,
            point = VK_POLYGON_MODE_POINT,
        };
        enum class CullMode : std::uint32_t
        {
            front = VK_CULL_MODE_FRONT_BIT,
            back = VK_CULL_MODE_BACK_BIT
        };
        enum class FrontFace : std::uint32_t
        {
            counter_clockwise = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            clockwise = VK_FRONT_FACE_CLOCKWISE
        };
        enum class Topology : std::uint32_t
        {
            point_list = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
            line_list = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
            line_strip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
            triangle_list = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            triangle_strip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
            triangle_fan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
        };

        PolygonMode polygon_mode;
        CullMode cull_mode;
        FrontFace front_face;
        Topology topology;

        bool primitive_restart = false;
    };
    struct DepthProperties
    {
        enum class CompareOperator : std::uint32_t
        {
            never = VK_COMPARE_OP_NEVER,
            less = VK_COMPARE_OP_LESS,
            equal = VK_COMPARE_OP_EQUAL,
            less_or_equal = VK_COMPARE_OP_LESS_OR_EQUAL,
            greater = VK_COMPARE_OP_GREATER,
            not_equal = VK_COMPARE_OP_NOT_EQUAL,
            greater_or_equal = VK_COMPARE_OP_GREATER_OR_EQUAL,
            always = VK_COMPARE_OP_ALWAYS,
        };

        bool depth_test = false;
        bool depth_write = false;
        bool depth_bounds_test = false;
        bool depth_bias = false;
        bool depth_clamp = false;

        float min_depth_bounds;
        float max_depth_bounds;

        float depth_bias_constantFactor;
        float depth_bias_clamp;
        float depth_bias_slope_factor;

        CompareOperator compare_operator;
    };
    struct StencilProperties
    {
        bool stencil_test = false;

        VkStencilOpState front;
        VkStencilOpState back;
    };
    struct MultisamplingProperties
    {
        enum class SampleCount
        {
            _1 = VK_SAMPLE_COUNT_1_BIT,
            _2 = VK_SAMPLE_COUNT_2_BIT,
            _4 = VK_SAMPLE_COUNT_4_BIT,
            _8 = VK_SAMPLE_COUNT_8_BIT,
            _16 = VK_SAMPLE_COUNT_16_BIT,
            _32 = VK_SAMPLE_COUNT_32_BIT,
            _64 = VK_SAMPLE_COUNT_64_BIT,
        };

        SampleCount rasterization_samples;
        bool sample_shading = false;
        bool alpha_to_coverage = false;
        bool alpha_to_one = false;

        float min_sample_shading;
        const VkSampleMask* pSampleMask;
    };

    Graphics(const PrimitiveProperties& primitive_properties_a,
             const DepthProperties& depth_properties_a,
             const StencilProperties& stencil_properties_a,
             const MultisamplingProperties& multisampling_properties_a);
    ~Graphics();

private:
    VkPipeline vk_pipeline;
    VkPipelineLayout vk_pipeline_layout;
};
} // namespace lx::gpu::pipelines