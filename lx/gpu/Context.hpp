#pragma once

// lx
#include <lx/Windower.hpp>
#include <lx/common/non_constructible.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/common/out.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/Device.hpp>

namespace lx::gpu {
class Context : public lx::common::non_copyable
{
public:
    struct s : private lx::common::non_constructible
    {
        static constexpr std::size_t devices_buffer_capacity = 5u;
    };

    template<typename Type> [[nodiscard]] Type* create(const lx::devices::GPU& gpu_a,
                                                       const lx::Canvas<lx::Windower::framed>* canvas_a,
                                                       typename const Type::Properties& properties_a) = delete;
    template<typename Type> void destroy(lx::common::out<Type*> obj) = delete;

private:
    lx::containers::Vector<lx::gpu::Device, s::devices_buffer_capacity> devices;
};

template<> inline [[nodiscard]] lx::gpu::Device* Context::create<lx::gpu::Device>(const lx::devices::GPU& gpu_a,
                                                                                  const lx::Canvas<lx::Windower::framed>* canvas_a,
                                                                                  const lx::gpu::Device::Properties& properties_a)
{
    lx::gpu::Device device;
    device.create(gpu_a, *canvas_a, properties_a);

    if (true == device.is_created())
    {
        this->devices.emplace_back(std::move(device));
        return &(this->devices.get_back());
    }

    return nullptr;
}

template<> inline void Context::destroy<lx::gpu::Device>(lx::common::out<lx::gpu::Device*> device_a)
{
    (*device_a)->destroy();
    (*device_a) = nullptr;
}
} // namespace lx::gpu