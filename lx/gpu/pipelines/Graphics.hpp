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
        std::span<Viewport<float>> viewports;
        std::span<lx::common::Rect<std::int32_t, std::int32_t>> scissors;

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
            /*
typedef enum VkBlendOp {
    VK_BLEND_OP_ADD = 0,
    VK_BLEND_OP_SUBTRACT = 1,
    VK_BLEND_OP_REVERSE_SUBTRACT = 2,
    VK_BLEND_OP_MIN = 3,
    VK_BLEND_OP_MAX = 4,
    VK_BLEND_OP_ZERO_EXT = 1000148000,
    VK_BLEND_OP_SRC_EXT = 1000148001,
    VK_BLEND_OP_DST_EXT = 1000148002,
    VK_BLEND_OP_SRC_OVER_EXT = 1000148003,
    VK_BLEND_OP_DST_OVER_EXT = 1000148004,
    VK_BLEND_OP_SRC_IN_EXT = 1000148005,
    VK_BLEND_OP_DST_IN_EXT = 1000148006,
    VK_BLEND_OP_SRC_OUT_EXT = 1000148007,
    VK_BLEND_OP_DST_OUT_EXT = 1000148008,
    VK_BLEND_OP_SRC_ATOP_EXT = 1000148009,
    VK_BLEND_OP_DST_ATOP_EXT = 1000148010,
    VK_BLEND_OP_XOR_EXT = 1000148011,
    VK_BLEND_OP_MULTIPLY_EXT = 1000148012,
    VK_BLEND_OP_SCREEN_EXT = 1000148013,
    VK_BLEND_OP_OVERLAY_EXT = 1000148014,
    VK_BLEND_OP_DARKEN_EXT = 1000148015,
    VK_BLEND_OP_LIGHTEN_EXT = 1000148016,
    VK_BLEND_OP_COLORDODGE_EXT = 1000148017,
    VK_BLEND_OP_COLORBURN_EXT = 1000148018,
    VK_BLEND_OP_HARDLIGHT_EXT = 1000148019,
    VK_BLEND_OP_SOFTLIGHT_EXT = 1000148020,
    VK_BLEND_OP_DIFFERENCE_EXT = 1000148021,
    VK_BLEND_OP_EXCLUSION_EXT = 1000148022,
    VK_BLEND_OP_INVERT_EXT = 1000148023,
    VK_BLEND_OP_INVERT_RGB_EXT = 1000148024,
    VK_BLEND_OP_LINEARDODGE_EXT = 1000148025,
    VK_BLEND_OP_LINEARBURN_EXT = 1000148026,
    VK_BLEND_OP_VIVIDLIGHT_EXT = 1000148027,
    VK_BLEND_OP_LINEARLIGHT_EXT = 1000148028,
    VK_BLEND_OP_PINLIGHT_EXT = 1000148029,
    VK_BLEND_OP_HARDMIX_EXT = 1000148030,
    VK_BLEND_OP_HSL_HUE_EXT = 1000148031,
    VK_BLEND_OP_HSL_SATURATION_EXT = 1000148032,
    VK_BLEND_OP_HSL_COLOR_EXT = 1000148033,
    VK_BLEND_OP_HSL_LUMINOSITY_EXT = 1000148034,
    VK_BLEND_OP_PLUS_EXT = 1000148035,
    VK_BLEND_OP_PLUS_CLAMPED_EXT = 1000148036,
    VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT = 1000148037,
    VK_BLEND_OP_PLUS_DARKER_EXT = 1000148038,
    VK_BLEND_OP_MINUS_EXT = 1000148039,
    VK_BLEND_OP_MINUS_CLAMPED_EXT = 1000148040,
    VK_BLEND_OP_CONTRAST_EXT = 1000148041,
    VK_BLEND_OP_INVERT_OVG_EXT = 1000148042,
    VK_BLEND_OP_RED_EXT = 1000148043,
    VK_BLEND_OP_GREEN_EXT = 1000148044,
    VK_BLEND_OP_BLUE_EXT = 1000148045,
    VK_BLEND_OP_MAX_ENUM = 0x7FFFFFFF
} VkBlendOp;

typedef enum VkBlendFactor {
    VK_BLEND_FACTOR_ZERO = 0,
    VK_BLEND_FACTOR_ONE = 1,
    VK_BLEND_FACTOR_SRC_COLOR = 2,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR = 3,
    VK_BLEND_FACTOR_DST_COLOR = 4,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR = 5,
    VK_BLEND_FACTOR_SRC_ALPHA = 6,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA = 7,
    VK_BLEND_FACTOR_DST_ALPHA = 8,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA = 9,
    VK_BLEND_FACTOR_CONSTANT_COLOR = 10,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = 11,
    VK_BLEND_FACTOR_CONSTANT_ALPHA = 12,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA = 13,
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE = 14,
    VK_BLEND_FACTOR_SRC1_COLOR = 15,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR = 16,
    VK_BLEND_FACTOR_SRC1_ALPHA = 17,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA = 18,
    VK_BLEND_FACTOR_MAX_ENUM = 0x7FFFFFFF
} VkBlendFactor;

typedef struct VkPipelineColorBlendAttachmentState {
    VkBool32                 blendEnable;
    VkBlendFactor            srcColorBlendFactor;
    VkBlendFactor            dstColorBlendFactor;
    VkBlendOp                colorBlendOp;
    VkBlendFactor            srcAlphaBlendFactor;
    VkBlendFactor            dstAlphaBlendFactor;
    VkBlendOp                alphaBlendOp;
    VkColorComponentFlags    colorWriteMask;
} VkPipelineColorBlendAttachmentState;

            */
        } blend;
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