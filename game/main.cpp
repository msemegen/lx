#include <lx/app.hpp>
#include <lx/gpu/Device.hpp>
#include <lx/utils/logger.hpp>

void lx::app::setup(lx::common::out<lx::app::Config> config_a)
{
    // log config
    config_a->log.console = true;
    config_a->log.path = "log.txt";

    // app config
    config_a->app.name = "test game";

    // vulkan config
    config_a->vulkan.validation.enabled = true;
    config_a->vulkan.validation.severity = lx::app::Config::vulkan::validation::Severity::verbose;
    config_a->vulkan.validation.kind =
        lx::app::Config::vulkan::validation::Kind::coretness | lx::app::Config::vulkan::validation::Kind::device_address_binding |
        lx::app::Config::vulkan::validation::Kind::general | lx::app::Config::vulkan::validation::Kind::performance;
}

std::int32_t lx::app::entry_point(std::span<const lx::devices::Display> displays_a,
                                  std::span<const lx::devices::GPU> gpus_a,
                                  gpu::Context& graphics_context,
                                  lx::Windower& windower_a,
                                  std::string_view cmd_line_a)
{
    using namespace lx::common;
    using namespace lx::gpu;
    using namespace lx::utils;

    log_set_filter(logger::dbg | logger::inf | logger::err);

    log_inf("Command line args: \"{}\"", cmd_line_a);
    log_inf("Displays count: {}", displays_a.size());
    log_inf("GPUs count: {}", gpus_a.size());

    for (auto q : gpus_a[0].queue_families)
    {
        log_inf("Q: kind: {}. count: {}, index: {}", static_cast<std::uint32_t>(q.kind), q.count, q.index);
    }

    {
        auto canvas1 = windower_a.create<Windower::framed>(
            displays_a[0], Canvas<Windower::framed>::Properties { .title = gpus_a[0].name, .size { .w = 800u, .h = 600u } });

        auto canvas2 = windower_a.create<Windower::framed>(
            displays_a[1], Canvas<Windower::framed>::Properties { .title = gpus_a[0].name, .size { .w = 800u, .h = 600u } });

        if (true == canvas1->is_created() && true == canvas2->is_created())
        {
            windower_a.events.size_size.register_callback();
            windower_a.events.position_change.register_callback();

            log_inf("Canvases created");
            auto gpu_device1 = graphics_context.create<Device>(
                gpus_a[0],
                canvas1,
                Device::Properties { .features = Device::Feature::none,
                                     .queue_families { std::array {
                                         Device::QueueFamily { .kind = Device::QueueFamily::graphics | Device::QueueFamily::transfer,
                                                               .count = 1u,
                                                               .priorities { 1.0f },
                                                               .presentation = true } } },
                                     .swap_chain { .format = Device::SwapChain::Format::r8g8b8a8_srgb,
                                                   .color_space = Device::SwapChain::ColorSpace::srgb_nonlinear_khr,
                                                   .mode = Device::SwapChain::Mode::fifo,
                                                   .images_count = 2u } });

            //auto gpu_device2 = graphics_context.create<Device>(
            //    gpus_a[1],
            //    canvas2,
            //    Device::Properties { .features = Device::Feature::none,
            //                         .queue_families { std::array {
            //                             Device::QueueFamily { .kind = Device::QueueFamily::graphics | Device::QueueFamily::transfer,
            //                                                   .count = 1u,
            //                                                   .priorities { 1.0f },
            //                                                   .presentation = true } } },
            //                         .swap_chain { .format = Device::SwapChain::Format::r8g8b8a8_srgb,
            //                                       .color_space = Device::SwapChain::ColorSpace::srgb_nonlinear_khr,
            //                                       .mode = Device::SwapChain::Mode::fifo,
            //                                       .images_count = 2u } });

            if (true == gpu_device1->is_created()/* && true == gpu_device2->is_created()*/)
            {
                log_inf("GPU context created");

                windower_a.set_visible(canvas1, true);
                windower_a.set_visible(canvas2, true);

                bool c1 = false;
                bool c2 = false;

                do
                {
                    c1 = windower_a.update(canvas1);
                    c2 = windower_a.update(canvas2);
                } while (true == c1 || true == c2);
            }
            else
            {
                log_err("GPU context not created");
            }

            if (true == gpu_device1->is_created() /*&& true == gpu_device2->is_created()*/)
            {
                log_inf("GPU context destroyed");
            }

            graphics_context.destroy(out(gpu_device1));
            //graphics_context.destroy(out(gpu_device2));
        }

        windower_a.destroy(out(canvas1));
        windower_a.destroy(out(canvas2));
    }

    log_inf("Window destroyed");

    return 0;
}
