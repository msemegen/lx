/*
 *   Name: main.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// lxf
#include <lxf/Windower.hpp>
#include <lxf/config.hpp>
#include <lxf/entry_point.hpp>
#include <lxf/renderer.hpp>

using namespace ::lxf;
using namespace ::lxf::common;
using namespace ::lxf::utils;

// logger config
std::string_view config::log::file_path = "log.log";
logger::Severity config::log::severity = logger::Severity::omg | logger::Severity::error | logger::Severity::info | logger::Severity::debug;
bool config::log::console = true;

// app config
std::string_view config::app::name = "eva";
Version config::app::version = Version { 0u, 0u, 1u };

// engine config
std::string_view config::engine::name = "lxe";
Version config::engine::version = Version { 0u, 0u, 1u };

// vulkan config
bool config::vulkan::validation_layer::enabled = true;
config::vulkan::validation_layer::Severity config::vulkan::validation_layer::severity =
    config::vulkan::validation_layer::Severity::info | config::vulkan::validation_layer::Severity::warning |
    config::vulkan::validation_layer::Severity::error | config::vulkan::validation_layer::Severity::verbose;
config::vulkan::validation_layer::Type config::vulkan::validation_layer::type =
    config::vulkan::validation_layer::Type::general | config::vulkan::validation_layer::Type::performance |
    config::vulkan::validation_layer::Type::validation | config::vulkan::validation_layer::Type::device_address_binding;

Version config::vulkan::version = Version { VK_API_VERSION_1_3 };

constexpr std::string_view module_name = "game:main";

std::int64_t
lxf::entry_point(std::string_view, std::span<device::GPU> gpus_a, std::span<device::Display> displays_a, Windower* p_windower_a)
{
    const auto gpu_properties = gpus_a[0].get_properties();
    constexpr device::Filter<device::GPU>::Requirements::Queue_family queue_families_req[] = {
        { .kind = device::GPU::Properties::Queue_family::graphics | device::GPU::Properties::Queue_family::transfer, .count = 1u },
        { .kind = device::GPU::Properties::Queue_family::video_decode, .count = 1u }
    };
    constexpr std::string_view extensions_req[] = { "VK_KHR_swapchain", "VK_EXT_pageable_device_local_memory", "VK_EXT_memory_priority" };

    const device::Filter<device::GPU>::Requirements::Limit limits_req[] = {
        { .kind = device::Filter<device::GPU>::Requirements::Limit::max_image_dimension_1d,
          .value = device::Filter<device::GPU>::Requirements::Limit::greater_than(5u) }
    };

    auto integrated_gpus = device::filter<device::GPU>(
        gpus_a,
        { .kind = device::GPU::integrated, .queue_families = queue_families_req, .extensions = extensions_req, .limits = limits_req });

    auto discrete_gpus = device::filter<device::GPU>(
        gpus_a,
        { .kind = device::GPU::discrete, .queue_families = queue_families_req, .extensions = extensions_req, .limits = limits_req });

    auto displays = device::filter<device::Display>(displays_a, { .kind = device::Display::primary | device::Display::additional });

    if (false == displays.empty() && (false == discrete_gpus.empty() || false == integrated_gpus.empty()))
    {
        logger::write_line(logger::info, module_name, "Found suitable configuration.");

        const device::GPU* p_gpu = false == discrete_gpus.empty() ? discrete_gpus[0] : integrated_gpus[0];
        const device::Display* p_display = displays[0];

        logger::write_line(logger::info, module_name, "Selected GPU: {}.", p_gpu->get_properties().name);
        logger::write_line(logger::info, module_name, "Selected display: {}", p_display->get_properties().name);

        canvas::Windowed* p_main_window =
            p_windower_a->create<canvas::Windowed>(displays[0],
                                                   { .title = "test",
                                                     .position = canvas::Position::centered,
                                                     .size = canvas::Size::custom | Size<std::uint16_t> { .w = 800u, .h = 600u } });

        if (true == p_main_window->is_created())
        {
            constexpr std::string_view extensions[] = { "VK_KHR_swapchain",
                                                        "VK_EXT_pageable_device_local_memory",
                                                        "VK_EXT_memory_priority" };

            constexpr renderer::Context::Properties::Queue_family context_qf[] = {
                { .kind = renderer::Context::Properties::Queue_family::graphics | renderer::Context::Properties::Queue_family::transfer,
                  .count = 1u }
            };

            renderer::Context* p_context =
                renderer::create<renderer::Context>(p_gpu, { .features = {}, .queue_families = context_qf, .extensions = extensions });

            p_main_window->set_visible(true);

            while (true == p_main_window->update())
            {
            }

            renderer::destroy<renderer::Context>(&p_context);

            p_windower_a->destroy(&p_main_window);
        }

        return -1ll;
    }

    return 0ll;
}