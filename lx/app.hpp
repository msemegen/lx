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
            bool console = false;
        } log;

        struct app
        {
            lx::containers::String<char> name;
            lx::common::Version version;
        } app;

        struct vulkan
        {
            bool enable_validation = false;
        } vulkan;
    };

    static void setup(common::out<Config> config_a);
    static std::int32_t entry_point(std::span<const devices::Display> displays_a,
                                    std::span<const devices::GPU> gpus_a,
                                    lx::gpu::Context& graphics_context_a,
                                    lx::Windower& window_a,
                                    std::string_view cmd_line_a);
};
} // namespace lx