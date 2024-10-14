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
logger::Severity config::log::severity = logger::Severity::omg | logger::Severity::error | logger::Severity::debug;
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

Int64 lxf::entry_point(std::string_view,
                       const std::vector<device::GPU>& gpus_a,
                       const std::vector<device::Display>& displays_a,
                       Windower* p_windower_a)
{
    const std::vector<device::GPU> avaliable_integrated_gpus =
        device::filter<device::GPU>(gpus_a, device::GPU::integrated, device::GPU::Queue::graphics, device::GPU::Feature::none);
    const std::vector<device::GPU> avaliable_discrete_gpus =
        device::filter<device::GPU>(gpus_a, device::GPU::discrete, device::GPU::Queue::graphics, device::GPU::Feature::none);
    const std::vector<device::Display> avaliable_displays = device::filter<device::Display>(displays_a, { .w = 800u, .h = 600u }, 32u);

    if ((false == avaliable_integrated_gpus.empty() || false == avaliable_discrete_gpus.empty()) && false == avaliable_displays.empty())
    {
        const auto gpu = false == avaliable_discrete_gpus.empty() ? avaliable_discrete_gpus[0] : avaliable_integrated_gpus[0];

        logger::write_line(logger::Level::info,
                           module_name,
                           "Selected GPU: {}, selected screen: {}",
                           gpu.get_descriptor().name,
                           avaliable_displays[0].get_name());

        canvas::Windowed* p_main_window =
            p_windower_a->create<canvas::Windowed>(avaliable_displays[0],
                                                   { .title = "test",
                                                     .position = canvas::Position::centered,
                                                     .size = canvas::Size::custom | Size<Uint16> { .w = 800u, .h = 600u } });

        if (true == p_main_window->is_created())
        {
            const device::GPU::Queue qs[] = { device::GPU::Queue { .kind = device::GPU::Queue::graphics, .count = 1u } };
            renderer::Context* p_rendering_context = renderer::create<renderer::Context>(gpu, qs, device::GPU::Feature::none);

            if (nullptr != p_rendering_context)
            {
                p_main_window->set_visible(true);
                logger::write_line(logger::Level::info, module_name, "Main window created.");

                logger::write_line(logger::Level::info, module_name, "Application started.");

                while (true == p_main_window->update())
                {
                }

                logger::write_line(logger::Level::info, module_name, "Application stopped.");

                renderer::destroy<renderer::Context>(&p_rendering_context);
            }
        }
        else
        {
            logger::write_line(logger::Level::info, module_name, "Cannot create rendering window.");
        }

        p_windower_a->destroy(&p_main_window);
    }
    else
    {
        if (true == avaliable_integrated_gpus.empty() && true == avaliable_discrete_gpus.empty())
        {
            logger::write_line(logger::Level::error, module_name, "No compatible GPU detected");
            return -1;
        }

        if (true == avaliable_displays.empty())
        {
            logger::write_line(logger::Level::omg, module_name, "No compatible display detected");
            return -1;
        }
    }

    return 0ll;
}