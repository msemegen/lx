#include <lx/app.hpp>
#include <lx/gpu/Device.hpp>
#include <lx/utils/logger.hpp>

// test
#include <lx/gpu/CommandList.hpp>

class App : public lx::Windower::Events::PositionChange::Callback
{
public:
    virtual bool on_position_change(std::uint64_t pos_x_a, std::uint64_t pos_y_a) override
    {
        printf("position change!!!\n");
        return true;
    }
};

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
    config_a->vulkan.validation.kind = lx::app::Config::vulkan::validation::coretness |
                                       lx::app::Config::vulkan::validation::device_address_binding |
                                       lx::app::Config::vulkan::validation::general | lx::app::Config::vulkan::validation::performance;
}

void foo(const lx::gpu::CommandList<lx::gpu::command_list::transfer>& command_list) {}

std::int32_t lx::app::entry_point(std::span<const lx::devices::Display> displays_a,
                                  std::span<const lx::devices::GPU> gpus_a,
                                  gpu::Context& graphics_context_a,
                                  lx::Windower& windower_a,
                                  std::string_view cmd_line_a)
{
    using namespace lx::common;
    using namespace lx::gpu;
    using namespace lx::utils;

    App app;

    log_set_filter(logger::dbg | logger::inf | logger::err);

    log_inf("Command line args: \"{}\"", cmd_line_a);
    log_inf("Displays count: {}", displays_a.size());
    log_inf("GPUs count: {}", gpus_a.size());

    for (auto gpu : gpus_a)
    {
        log_inf("GPU: {}", gpu.name.get_cstring());
        for (const auto q : gpu.queue_families)
        {
            log_inf("\tQ: kind: {}. members: {}, index: {}", static_cast<std::uint32_t>(q.kind), q.members, q.index);
        }
    }

    {
        auto canvas1 = windower_a.create<canvas::framed>(
            displays_a[0], Canvas<canvas::framed>::Properties { .title = gpus_a[0].name, .size { .w = 800u, .h = 600u } });

        auto canvas2 = windower_a.create<canvas::framed>(
            displays_a[0], Canvas<canvas::framed>::Properties { .title = gpus_a[0].name, .size { .w = 800u, .h = 600u } });

        if (true == canvas1->is_created() && true == canvas2->is_created())
        {
            /*
                    .queue_families { std::array { Device::QueueFamily {
                        .kind = Device::QueueFamily::graphics | Device::QueueFamily::compute | Device::QueueFamily::transfer,
                        .members = 4u,
                        .priorities { 1.0f, 1.0f, 1.0f, 1.0f },
                        .presentation = true } } }
            */
            log_inf("Canvases created");
            auto gpu_device1 = graphics_context_a.create<Device>(
                gpus_a[0], canvas1, Device::Properties { .features = Device::Feature::none, .extensions {} });

            if (true == gpu_device1.is_created())
            {
                auto swap_chain = gpu_device1.create<lx::gpu::SwapChain>({ .format = SwapChain::Format::r8g8b8a8_srgb,
                                                                           .color_space = SwapChain::ColorSpace::srgb_nonlinear_khr,
                                                                           .mode = SwapChain::Mode::fifo,
                                                                           .extent { .w = 800u, .h = 600u },
                                                                           .images_count = 2u });
                auto rendering_queue =
                    gpu_device1.create<lx::gpu::Queue>({ .kind = Queue::graphics | Queue::transfer, .presentation = true });
                auto command_pool = gpu_device1.create<lx::gpu::CommandPool>(rendering_queue);
                auto command_list = gpu_device1.create<lx::gpu::CommandList<command_list::graphics | command_list::transfer>>(command_pool);

                foo(command_list);
                // foo(static_cast<const lx::gpu::CommandList<command_list::graphics>&>(command_list));

                auto transfer_list = static_cast<lx::gpu::CommandList<command_list::transfer>>(command_list);
               // auto buffer = gpu_device1.create<lx::gpu::Buffer>(&transfer_list, lx::gpu::Buffer::Properties {});

                // auto command_pool = gpu_device1.create<lx::gpu::CommandPool>(rendering_queue);
                // auto command_buffer = gpu_device1.create<lx::gpu::CommandBuffer>(command_pool);

                /*
                 *  .swap_chain {  }
                 */

                // auto gpu_device2 = graphics_context.create<Device>(
                //     gpus_a[1],
                //     canvas2,
                //     Device::Properties { .features = Device::Feature::none,
                //                          .queue_families { std::array {
                //                              Device::QueueFamily { .kind = Device::QueueFamily::graphics | Device::QueueFamily::transfer,
                //                                                    .count = 1u,
                //                                                    .priorities { 1.0f },
                //                                                    .presentation = true } } },
                //                          .swap_chain { .format = Device::SwapChain::Format::r8g8b8a8_srgb,
                //                                        .color_space = Device::SwapChain::ColorSpace::srgb_nonlinear_khr,
                //                                        .mode = Device::SwapChain::Mode::fifo,
                //                                        .images_count = 2u } });

                if (true == gpu_device1.is_created() /* && true == gpu_device2->is_created()*/)
                {
                    log_inf("GPU context created");

                    windower_a.set_visible(canvas1, true);
                    windower_a.set_visible(canvas2, true);

                    windower_a.events.position_change.register_callback(inout(canvas1), &app);

                    bool c1 = false;
                    bool c2 = false;

                    // auto p = gpu_device1->create<Graphics>(
                    //     { .primitive { .polygon_mode = Graphics::Primitive::PolygonMode::fill,
                    //                    .cull_mode = Graphics::Primitive::CullMode::front | Graphics::Primitive::CullMode::back,
                    //                    .front_face = Graphics::Primitive::FrontFace::clockwise,
                    //                    .topology = Graphics::Primitive::Topology::triangle_list },
                    //       .depth {},
                    //       .stencil {},
                    //       .multisampling {},
                    //       .blend {},
                    //       .vertex_input {},
                    //       .shaders {},
                    //       .clips {} });

                    do
                    {
                        c1 = windower_a.update(canvas1);
                        c2 = windower_a.update(canvas2);
                    } while (true == c1 || true == c2);

                    // gpu_device1->destroy(out(p));
                }
                else
                {
                    log_err("GPU context not created");
                }

                if (true == gpu_device1.is_created() /*&& true == gpu_device2->is_created()*/)
                {
                    log_inf("GPU context destroyed");
                }

                graphics_context_a.destroy(out(gpu_device1));
                // graphics_context.destroy(out(gpu_device2));
            }
        }

        windower_a.destroy(out(canvas1));
        windower_a.destroy(out(canvas2));
    }

    log_inf("Window destroyed");

    return 0;
}
