#pragma once

// lx
#include <lx/common/Rect.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/gpu/Viewport.hpp>
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <array>
#include <span>

namespace lx::gpu::pipelines {
class Graphics : private lx::common::non_copyable
{
public:
    /*
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
        struct Shader
        {
            enum class Kind : std::uint64_t
            {
                vertex = VK_SHADER_STAGE_VERTEX_BIT,
                fragment = VK_SHADER_STAGE_FRAGMENT_BIT
            };

            using enum Kind;

            Kind kind;
            lx::containers::Vector<std::byte> code;
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
            enum class Compare : std::uint32_t
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

            bool test = false;
            bool write = false;
            bool bounds_test = false;
            bool bias = false;
            bool clamp = false;

            float min_bounds;
            float max_bounds;

            float bias_constant_factor;
            float bias_clamp;
            float bias_slope_factor;

            Compare compare;
        } depth;
        struct Stencil
        {
            struct State
            {
                enum class Compare : std::uint32_t
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
                enum class Operation
                {
                    keep = VK_STENCIL_OP_KEEP,
                    zero = VK_STENCIL_OP_ZERO,
                    replace = VK_STENCIL_OP_REPLACE,
                    increment_and_clamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
                    decrement_and_clamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
                    invert = VK_STENCIL_OP_INVERT,
                    increment_and_wrap = VK_STENCIL_OP_INCREMENT_AND_WRAP,
                    decrement_and_wrap = VK_STENCIL_OP_DECREMENT_AND_WRAP,
                };
                Operation fail;
                Operation pass;
                Operation depth_fail;
                Compare compare;
                std::uint32_t compare_mask;
                std::uint32_t write_mask;
                std::uint32_t reference;
            };
            bool test = false;

            State front;
            State back;
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
                    source_over_destination = VK_BLEND_OP_SRC_OVER_EXT,
                    destination_over_source = VK_BLEND_OP_DST_OVER_EXT,
                    source_in = VK_BLEND_OP_SRC_IN_EXT,
                    destination_in = VK_BLEND_OP_DST_IN_EXT,
                    source_out = VK_BLEND_OP_SRC_OUT_EXT,
                    destination_out = VK_BLEND_OP_DST_OUT_EXT,
                    source_atop = VK_BLEND_OP_SRC_ATOP_EXT,
                    destination_atop = VK_BLEND_OP_DST_ATOP_EXT,
                    xor_ = VK_BLEND_OP_XOR_EXT,
                    multiply = VK_BLEND_OP_MULTIPLY_EXT,
                    screen = VK_BLEND_OP_SCREEN_EXT,
                    overlay = VK_BLEND_OP_OVERLAY_EXT,
                    darken = VK_BLEND_OP_DARKEN_EXT,
                    lighten = VK_BLEND_OP_LIGHTEN_EXT,
                    color_dodge = VK_BLEND_OP_COLORDODGE_EXT,
                    color_burn = VK_BLEND_OP_COLORBURN_EXT,
                    hard_light = VK_BLEND_OP_HARDLIGHT_EXT,
                    soft_light = VK_BLEND_OP_SOFTLIGHT_EXT,
                    difference = VK_BLEND_OP_DIFFERENCE_EXT,
                    exclusion = VK_BLEND_OP_EXCLUSION_EXT,
                    invert = VK_BLEND_OP_INVERT_EXT,
                    invert_rgb = VK_BLEND_OP_INVERT_RGB_EXT,
                    lienear_dodge = VK_BLEND_OP_LINEARDODGE_EXT,
                    linear_burn = VK_BLEND_OP_LINEARBURN_EXT,
                    vivid_light = VK_BLEND_OP_VIVIDLIGHT_EXT,
                    linear_light = VK_BLEND_OP_LINEARLIGHT_EXT,
                    pin_light = VK_BLEND_OP_PINLIGHT_EXT,
                    hard_mix = VK_BLEND_OP_HARDMIX_EXT,
                    hsl_hue = VK_BLEND_OP_HSL_HUE_EXT,
                    hsl_saturation = VK_BLEND_OP_HSL_SATURATION_EXT,
                    hsl_color = VK_BLEND_OP_HSL_COLOR_EXT,
                    hsl_luminosity = VK_BLEND_OP_HSL_LUMINOSITY_EXT,
                    plus = VK_BLEND_OP_PLUS_EXT,
                    plus_clamped = VK_BLEND_OP_PLUS_CLAMPED_EXT,
                    plus_clamped_alpha = VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT,
                    plus_darker = VK_BLEND_OP_PLUS_DARKER_EXT,
                    minus = VK_BLEND_OP_MINUS_EXT,
                    minus_clamped = VK_BLEND_OP_MINUS_CLAMPED_EXT,
                    contrast = VK_BLEND_OP_CONTRAST_EXT,
                    invert_openvg = VK_BLEND_OP_INVERT_OVG_EXT,
                    red = VK_BLEND_OP_RED_EXT,
                    green = VK_BLEND_OP_GREEN_EXT,
                    blue = VK_BLEND_OP_BLUE_EXT,
                };
                enum WriteMask : std::uint32_t
                {
                    r = VK_COLOR_COMPONENT_R_BIT,
                    g = VK_COLOR_COMPONENT_G_BIT,
                    b = VK_COLOR_COMPONENT_B_BIT,
                    a = VK_COLOR_COMPONENT_A_BIT
                };

                bool enable;
                WriteMask write_mask;

                struct Color
                {
                    Factor source_factor;
                    Factor destination_factor;
                    Operator opr;
                } color;
                struct Alpha
                {
                    Factor source_factor;
                    Factor destination_factor;
                    Operator opr;
                } alpha;
            };

            std::span<Attachment> attachments;
            std::array<float, 4u> constants;
        } blend;
        struct VertexInput
        {
            struct Binding
            {
            };
            struct Attribute
            {
            };

            std::span<Binding> bindings;
            std::span<Attribute> attributes;
        } vertex_input;

        std::span<Shader> shaders;
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