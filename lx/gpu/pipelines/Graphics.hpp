#pragma once

// lx
#include <lx/common/Rect.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/gpu/Viewport.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <span>

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

    struct Properties
    {
        struct Clip
        {
            Viewport<float> viewport;
            lx::common::Rect<std::int32_t, std::int32_t> scissors;
        };

        struct Primitive
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
        } primitive;
        struct Depth
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
        } depth;
        struct Stencil
        {
            /*
typedef enum VkStencilOp {
    VK_STENCIL_OP_KEEP = 0,
    VK_STENCIL_OP_ZERO = 1,
    VK_STENCIL_OP_REPLACE = 2,
    VK_STENCIL_OP_INCREMENT_AND_CLAMP = 3,
    VK_STENCIL_OP_DECREMENT_AND_CLAMP = 4,
    VK_STENCIL_OP_INVERT = 5,
    VK_STENCIL_OP_INCREMENT_AND_WRAP = 6,
    VK_STENCIL_OP_DECREMENT_AND_WRAP = 7,
    VK_STENCIL_OP_MAX_ENUM = 0x7FFFFFFF
} VkStencilOp;

typedef struct VkStencilOpState {
    VkStencilOp    failOp;
    VkStencilOp    passOp;
    VkStencilOp    depthFailOp;
    VkCompareOp    compareOp;
    uint32_t       compareMask;
    uint32_t       writeMask;
    uint32_t       reference;
} VkStencilOpState;
            */
            bool stencil_test = false;

            VkStencilOpState front;
            VkStencilOpState back;
        } stencil;
        struct Multisampling
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
        } multisampling;
        struct Blend
        {
            struct Attachment
            {
                enum class Factor : std::uint32_t
                {
                    zero = VK_BLEND_FACTOR_ZERO,
                    one = VK_BLEND_FACTOR_ONE,
                    source = VK_BLEND_FACTOR_SRC_COLOR,
                    one_minus_source = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
                    destination = VK_BLEND_FACTOR_DST_COLOR,
                    one_minus_destination = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
                    source_alpha = VK_BLEND_FACTOR_SRC_ALPHA,
                    one_minus_source_alpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                    destination_alpha = VK_BLEND_FACTOR_DST_ALPHA,
                    one_minus_destination_alpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
                    constant_color = VK_BLEND_FACTOR_CONSTANT_COLOR,
                    one_minus_constant_color = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
                    constant_alpha = VK_BLEND_FACTOR_CONSTANT_ALPHA,
                    one_minus_constant_alpha = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
                    source_alpha_saturate = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
                    source_1_color = VK_BLEND_FACTOR_SRC1_COLOR,
                    one_minus_source_1_color = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
                    source_1_alpha = VK_BLEND_FACTOR_SRC1_ALPHA,
                    one_minus_source_1_alpha = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,
                };
                enum class Operator : std::uint32_t
                {
                    add = VK_BLEND_OP_ADD,
                    substract = VK_BLEND_OP_SUBTRACT,
                    reverse_substract = VK_BLEND_OP_REVERSE_SUBTRACT,
                    min = VK_BLEND_OP_MIN,
                    max = VK_BLEND_OP_MAX,
                    zero = VK_BLEND_OP_ZERO_EXT,
                    source = VK_BLEND_OP_SRC_EXT,
                    estination = VK_BLEND_OP_DST_EXT,
                    VK_BLEND_OP_SRC_OVER_EXT,
                    VK_BLEND_OP_DST_OVER_EXT,
                    VK_BLEND_OP_SRC_IN_EXT,
                    VK_BLEND_OP_DST_IN_EXT,
                    VK_BLEND_OP_SRC_OUT_EXT,
                    VK_BLEND_OP_DST_OUT_EXT,
                    VK_BLEND_OP_SRC_ATOP_EXT,
                    VK_BLEND_OP_DST_ATOP_EXT,
                    VK_BLEND_OP_XOR_EXT,
                    VK_BLEND_OP_MULTIPLY_EXT,
                    VK_BLEND_OP_SCREEN_EXT,
                    VK_BLEND_OP_OVERLAY_EXT,
                    VK_BLEND_OP_DARKEN_EXT,
                    VK_BLEND_OP_LIGHTEN_EXT,
                    VK_BLEND_OP_COLORDODGE_EXT,
                    VK_BLEND_OP_COLORBURN_EXT,
                    VK_BLEND_OP_HARDLIGHT_EXT,
                    VK_BLEND_OP_SOFTLIGHT_EXT,
                    VK_BLEND_OP_DIFFERENCE_EXT,
                    VK_BLEND_OP_EXCLUSION_EXT,
                    VK_BLEND_OP_INVERT_EXT,
                    VK_BLEND_OP_INVERT_RGB_EXT,
                    VK_BLEND_OP_LINEARDODGE_EXT,
                    VK_BLEND_OP_LINEARBURN_EXT,
                    VK_BLEND_OP_VIVIDLIGHT_EXT,
                    VK_BLEND_OP_LINEARLIGHT_EXT,
                    VK_BLEND_OP_PINLIGHT_EXT,
                    VK_BLEND_OP_HARDMIX_EXT,
                    VK_BLEND_OP_HSL_HUE_EXT,
                    VK_BLEND_OP_HSL_SATURATION_EXT,
                    VK_BLEND_OP_HSL_COLOR_EXT,
                    VK_BLEND_OP_HSL_LUMINOSITY_EXT,
                    VK_BLEND_OP_PLUS_EXT,
                    VK_BLEND_OP_PLUS_CLAMPED_EXT,
                    VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT,
                    VK_BLEND_OP_PLUS_DARKER_EXT,
                    VK_BLEND_OP_MINUS_EXT,
                    VK_BLEND_OP_MINUS_CLAMPED_EXT,
                    VK_BLEND_OP_CONTRAST_EXT,
                    VK_BLEND_OP_INVERT_OVG_EXT,
                    VK_BLEND_OP_RED_EXT,
                    VK_BLEND_OP_GREEN_EXT,
                    VK_BLEND_OP_BLUE_EXT,
                };
                // typedef struct VkPipelineColorBlendAttachmentState
                //{
                //     VkBool32 blendEnable;
                //     VkBlendFactor srcColorBlendFactor;
                //     VkBlendFactor dstColorBlendFactor;
                //     VkBlendOp colorBlendOp;
                //     VkBlendFactor srcAlphaBlendFactor;
                //     VkBlendFactor dstAlphaBlendFactor;
                //     VkBlendOp alphaBlendOp;
                //     VkColorComponentFlags colorWriteMask;
                // } VkPipelineColorBlendAttachmentState;

                bool enable;

                struct Color
                {

                }color;
                struct Alpha
                {

                }alpha;

                Factor source_color_blend_factor;
                Factor destination_color_blend_factor;
                Operator color;
            };

            std::span<Attachment> attachments;
        } blend;

        std::span<Clip> clips;
    };

    bool is_created() const
    {
        return VK_NULL_HANDLE != this->vk_pipeline && VK_NULL_HANDLE != this->vk_pipeline_layout;
    }

private:
    void create(const Properties& properties_a);
    void destroy();

    VkPipeline vk_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout vk_pipeline_layout = VK_NULL_HANDLE;

    friend class Device;
};
} // namespace lx::gpu::pipelines