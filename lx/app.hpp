#pragma once

// lx
#include <lx/Windower.hpp>
#include <lx/common/Version.hpp>
#include <lx/common/non_constructible.hpp>
#include <lx/common/out.hpp>
#include <lx/containers/String.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/Display.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/Context.hpp>

// std
#include <cstdint>
#include <span>

namespace lx {
struct app : lx::common::non_constructible
{
    struct Config
    {
        struct log
        {
            lx::containers::String<char> path;
            bool console;
        } log;

        struct app
        {
            lx::containers::String<char> name;
            lx::common::Version version;
        } app;

        struct vulkan
        {
            struct instance
            {
                lx::containers::Vector<const char*> extensions;
                lx::containers::Vector<const char*> layers;
            } instance;

            struct validation
            {
                enum class Severity : std::uint32_t
                {
                    none = 0xFFFFFFFFu,
                    info = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
                    warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
                    error = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                    verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                };
                enum class Kind : std::uint32_t
                {
                    none = 0xFFFFFFFFu,
                    general = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
                    coretness = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
                    performance = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                    device_address_binding = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
                };

                using enum Kind;

                bool enabled;
                Severity severity;
                Kind kind;
            } validation;
        } vulkan;
    };
    static void setup(common::out<Config> config_a);
    static std::int32_t entry_point(std::span<const devices::Display> displays_a,
                                    std::span<const devices::GPU> gpus_a,
                                    lx::gpu::Context& graphics_context_a,
                                    lx::Windower& window_a,
                                    std::string_view cmd_line_a);
};

constexpr app::Config::vulkan::validation::Kind operator|(app::Config::vulkan::validation::Kind left_a,
                                                          app::Config::vulkan::validation::Kind right_a)
{
    return static_cast<app::Config::vulkan::validation::Kind>(static_cast<std::uint32_t>(left_a) | static_cast<std::uint32_t>(right_a));
}
} // namespace lx