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
    const char* extensions[] = { "VK_KHR_swapchain", "VK_EXT_pageable_device_local_memory", "VK_EXT_memory_priority" };

    std::vector<const device::GPU*> compatible_integrated_gpus;
    std::vector<const device::GPU*> compatible_discrete_gpus;
    std::vector<const device::Display*> compatible_displays;

    // find suitable configuration
    {
        constexpr device::Filter<device::GPU>::Requirements::Queue_family queue_families_requirments[] = {
            { .kind = device::GPU::Properties::Queue_family::graphics | device::GPU::Properties::Queue_family::transfer, .count = 1u },
            { .kind = device::GPU::Properties::Queue_family::video_decode, .count = 1u }
        };
        const device::Filter<device::GPU>::Requirements::Limit limits_requirments[] = {
            { .kind = device::Filter<device::GPU>::Requirements::Limit::max_image_dimension_2d,
              .value = device::Filter<device::GPU>::Requirements::Limit::greater_than(1024) }
        };

        compatible_integrated_gpus = device::filter<device::GPU>(gpus_a,
                                                                 { .kind = device::GPU::integrated,
                                                                   .queue_families = queue_families_requirments,
                                                                   .extensions = { extensions, 3u },
                                                                   .limits = limits_requirments });

        compatible_discrete_gpus = device::filter<device::GPU>(gpus_a,
                                                               { .kind = device::GPU::discrete,
                                                                 .queue_families = queue_families_requirments,
                                                                 .extensions = { extensions, 3u },
                                                                 .limits = limits_requirments });

        compatible_displays =
            device::filter<device::Display>(displays_a, { .kind = device::Display::primary | device::Display::additional });
    }

    if (false == compatible_integrated_gpus.empty() &&
        (false == compatible_discrete_gpus.empty() || false == compatible_integrated_gpus.empty()))
    {
        logger::write_line(logger::info, module_name, "Found suitable configuration.");

        const device::GPU* p_gpu = false == compatible_discrete_gpus.empty() ? compatible_discrete_gpus[0] : compatible_integrated_gpus[0];
        const device::Display* p_display = compatible_displays[0];

        logger::write_line(logger::info, module_name, "Selected GPU: {}.", p_gpu->get_properties().p_name);
        logger::write_line(logger::info, module_name, "Selected display: {}", p_display->get_properties().name);

        canvas::Windowed* p_main_window =
            p_windower_a->create<canvas::Windowed>(compatible_displays[0],
                                                   { .title = "test",
                                                     .position = canvas::Position::centered,
                                                     .size = canvas::Size::custom | Size<std::uint16_t> { .w = 800u, .h = 600u } });

        if (true == p_main_window->is_created())
        {
            const std::uint16_t qf_priority = 1;
            const renderer::Context::Properties::Queue_family context_queue_family[] = {
                { .kind = renderer::Context::Properties::Queue_family::graphics | renderer::Context::Properties::Queue_family::transfer,
                  .count = 1u,
                  .priorities = { &qf_priority, 1u } }
            };

            renderer::Context* p_context = renderer::create<renderer::Context>(
                p_gpu, { .features = {}, .queue_families = context_queue_family, .extensions = { extensions, 3u } });

            if (nullptr != p_context)
            {
                p_main_window->set_visible(true);

                while (true == p_main_window->update())
                {
                }

                renderer::destroy<renderer::Context>(&p_context);
            }
            else
            {
                logger::write_line(logger::error, module_name, "Cannot create rendering context.");
            }

            p_windower_a->destroy(&p_main_window);

            return 0ll;
        }
    }

    return -1ll;
}