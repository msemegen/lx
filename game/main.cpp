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
Version config::vulkan::version = Version { VK_API_VERSION_1_3 };

constexpr std::string_view module_name = "game:main";

std::int64_t
lxf::entry_point(std::string_view, std::span<device::GPU> gpus_a, std::span<device::Display> displays_a, Windower* p_windower_a)
{
    const auto gpu_properties = gpus_a[0].get_properties();
    constexpr device::Filter<device::GPU>::Requirements::Queue_family queue_families_req[] = {
        { .kind = device::GPU::Queue_family::graphics | device::GPU::Queue_family::transfer, .count = 1u },
        { .kind = device::GPU::Queue_family::video_decode, .count = 1u }
    };
    constexpr std::string_view extensions_req[] = { "VK_KHR_swapchain" };

    const device::Filter<device::GPU>::Requirements::Limit limits_req[] = {
        { .kind = device::Filter<device::GPU>::Requirements::Limit::max_image_dimension_1d,
          .value = device::Filter<device::GPU>::Requirements::Limit::greater_than(5u) }
    };

    auto gpus = device::filter<device::GPU>(
        gpus_a, { .kind = device::GPU::primary, .queue_families = queue_families_req, .extensions = extensions_req, .limits = limits_req });
    auto displays =
        device::filter<device::Display>(displays_a, { .kind = device::Display::Kind::primary | device::Display::Kind::additional });

    // const std::vector<const device::GPU*> avaliable_integrated_gpus = device::filter<device::GPU>(gpus_a,
    //                                 device::GPU::integrated,
    //                                 { { device::GPU::Properties::QueueFamily::graphics, 1u } },
    //                                 device::GPU::Feature::none,
    //                                 { "VK_EXT_multi_draw" });
    // const std::vector<const device::GPU*> avaliable_discrete_gpus = device::filter<device::GPU>(
    //     gpus_a, device::GPU::discrete, device::GPU::QueueFamily::graphics, device::GPU::Feature::none, { "VK_EXT_multi_draw" });
    // const std::vector<const device::Display*> avaliable_displays =
    //     device::filter<device::Display>(displays_a, { .w = 800u, .h = 600u }, 32u);
    //
    // if ((false == avaliable_integrated_gpus.empty() || false == avaliable_discrete_gpus.empty()) && false == avaliable_displays.empty())
    //{
    //     const auto p_gpu = false == avaliable_discrete_gpus.empty() ? avaliable_discrete_gpus[0] : avaliable_integrated_gpus[0];
    //     const auto p_display = avaliable_displays[0];
    //
    //     logger::write_line(
    //         logger::Level::info, module_name, "Selected GPU: {}, selected screen: {}", p_gpu->get_properties().name,
    //         p_display->get_name());
    //
    //     canvas::Windowed* p_main_window =
    //         p_windower_a->create<canvas::Windowed>(avaliable_displays[0],
    //                                                { .title = "test",
    //                                                  .position = canvas::Position::centered,
    //                                                  .size = canvas::Size::custom | Size<Uint16> { .w = 800u, .h = 600u } });
    //
    //     if (true == p_main_window->is_created())
    //     {
    //         std::pair<device::GPU::QueueFamily, std::span<common::Uint8>> qs; /* = {
    //             { device::GPU::QueueFamily { .kind = device::GPU::QueueFamily::graphics, .count = 1u }, std::span<common::Uint8> {
    //         static_cast<common::Uint8>(255u) } }
    //         };*/
    //
    //         renderer::Context* p_rendering_context = renderer::create<renderer::Context>(p_gpu, { qs }, device::GPU::Feature::none);
    //
    //         if (nullptr != p_rendering_context)
    //         {
    //             p_main_window->set_visible(true);
    //             logger::write_line(logger::Level::info, module_name, "Main window created.");
    //
    //             logger::write_line(logger::Level::info, module_name, "Application started.");
    //
    //             while (true == p_main_window->update())
    //             {
    //             }
    //
    //             logger::write_line(logger::Level::info, module_name, "Application stopped.");
    //
    //             renderer::destroy<renderer::Context>(&p_rendering_context);
    //         }
    //     }
    //     else
    //     {
    //         logger::write_line(logger::Level::info, module_name, "Cannot create rendering window.");
    //     }
    //
    //     p_windower_a->destroy(&p_main_window);
    // }
    // else
    //{
    //     if (true == avaliable_integrated_gpus.empty() && true == avaliable_discrete_gpus.empty())
    //     {
    //         logger::write_line(logger::Level::error, module_name, "No compatible GPU detected");
    //         return -1;
    //     }
    //
    //     if (true == avaliable_displays.empty())
    //     {
    //         logger::write_line(logger::Level::omg, module_name, "No compatible display detected");
    //         return -1;
    //     }
    // }

    return 0ll;
}