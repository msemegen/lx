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
logger::Severity config::log::severity = logger::Severity::omg | logger::Severity::error | logger::Severity::info;
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

std::int64_t lxf::entry_point(std::string_view,
                              std::span<const device::GPU*> gpus_a,
                              std::span<const device::Display*> displays_a,
                              Windower* p_windower_a)
{
    std::vector<const device::Display*> compatible_displays = device::filter(
        displays_a,
        device::filter::Display { .kind = device::filter::Display::primary, .logical_size { .w = 800u, .h = 600 }, .bits_per_pixel = 32u });

    if (false == compatible_displays.empty())
    {
        canvas::Windowed* p_main_window =
            p_windower_a->create<canvas::Windowed>(compatible_displays[0]->get_properties().logical_rect,
                                                   { .title = "test",
                                                     .position = canvas::Position::centered,
                                                     .size = canvas::Size::custom | Extent<std::uint16_t> { .w = 800u, .h = 600u } });
        if (nullptr != p_main_window)
        {
            const char* extensions[] = { "VK_KHR_swapchain", "VK_EXT_pageable_device_local_memory", "VK_EXT_memory_priority" };
            const device::filter::GPU::Limit limits[] = {
                { .kind = device::filter::GPU::Limit::max_image_dimension_2d, .value = device::filter::GPU::Limit::greater_or_equal(1024) },
                { .kind = device::filter::GPU::Limit::max_image_dimension_3d, .value = device::filter::GPU::Limit::greater_or_equal(1024) }
            };
            constexpr device::filter::GPU::Queue_family queue_families[] = {
                { .kind = device::filter::GPU::Queue_family::graphics | device::filter::GPU::Queue_family::transfer |
                          device::filter::GPU::Queue_family::presentation,
                  .count = 1u },
                { .kind = device::filter::GPU::Queue_family::video_decode, .count = 1u }
            };

            std::vector<const device::GPU*> compatible_discrete_gpus = device::filter(
                { gpus_a },
                p_main_window,
                device::filter::GPU { .kind = device::filter::GPU::discrete,
                                      .features = device::filter::GPU::Feature::depth_clamp,
                                      .queue_families = queue_families,
                                      .extensions = extensions,
                                      .limits = limits,
                                      .swap_chain = { .pixel = device::filter::GPU::Swap_chain::Pixel::b8g8r8a8_srgb,
                                                      .color_space = device::filter::GPU::Swap_chain::Color_space::srgb_nonlinear_khr,
                                                      .images_count = 2u } });

            std::vector<const device::GPU*> compatible_integrated_gpus = device::filter(
                { gpus_a },
                p_main_window,
                device::filter::GPU { .kind = device::filter::GPU::integrated,
                                      .features = device::filter::GPU::Feature::depth_clamp,
                                      .queue_families = queue_families,
                                      .extensions = extensions,
                                      .limits = limits,
                                      .swap_chain = { .pixel = device::filter::GPU::Swap_chain::Pixel::b8g8r8a8_srgb,
                                                      .color_space = device::filter::GPU::Swap_chain::Color_space::srgb_nonlinear_khr,
                                                      .images_count = 2u } });

            if (false == compatible_discrete_gpus.empty() || false == compatible_integrated_gpus.empty())
            {
                p_main_window->set_visible(true);

                const std::uint16_t queue_families_priorities[] = { 1u };
                const renderer::Context::Properties::Queue_family queue_family[] = {
                    { .kind = renderer::Context::Properties::Queue_family::graphics | renderer::Context::Properties::Queue_family::compute,
                      .count = 1u,
                      .priorities = queue_families_priorities }
                };

                renderer::Context* p_context = renderer::create<renderer::Context>(
                    false == compatible_discrete_gpus.empty() ? compatible_discrete_gpus[0] : compatible_integrated_gpus[0],
                    p_main_window,
                    { .queue_families = queue_family,
                      .extensions = extensions,
                      .swap_chain = { .pixel = renderer::Context::Properties::Swap_chain::Pixel::b8g8r8a8_srgb,
                                      .color_space = renderer::Context::Properties::Swap_chain::Color_space::srgb_nonlinear_khr,
                                      .mode = renderer::Context::Properties::Swap_chain::Mode::fifo,
                                      .images_count = 3u } });

                while (true == p_main_window->update())
                {
                }

                renderer::destroy<renderer::Context>(&p_context);
            }

            p_windower_a->destroy(&(p_main_window));
        }
    }

    return 0ll;
}