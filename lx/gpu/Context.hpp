#pragma once

// lx
#include <lx/Windower.hpp>
#include <lx/common/non_copyable.hpp>
#include <lx/common/out.hpp>
#include <lx/containers/Vector.hpp>
#include <lx/devices/GPU.hpp>
#include <lx/gpu/Device.hpp>

namespace lx::gpu {
class Context : public lx::common::non_copyable
{
public:
    template<typename Type> Type* create(const lx::devices::GPU& gpu_a,
                                         const lx::Canvas<lx::Windower::framed>* canvas_a,
                                         typename const Type::Properties& properties_a) = delete;
    template<typename Type> void destroy(lx::common::out<Type*> obj) = delete;

private:
    lx::containers::Vector<lx::gpu::Device*> devices;
};

template<> inline lx::gpu::Device* Context::create<lx::gpu::Device>(const lx::devices::GPU& gpu_a,
                                                                    const lx::Canvas<lx::Windower::framed>* canvas_a,
                                                                    const lx::gpu::Device::Properties& properties_a)
{
    auto device = new Device(gpu_a,
                             *canvas_a,
                             { .width = static_cast<std::uint32_t>(canvas_a->get_properties().size.w),
                               .height = static_cast<std::uint32_t>(canvas_a->get_properties().size.h) },
                             properties_a);

    if (true == device->is_created())
    {
        this->devices.push_back(device);
        return this->devices.get_back();
    }

    return nullptr;
}

template<> inline void Context::destroy<lx::gpu::Device>(lx::common::out<lx::gpu::Device*> device_a)
{
    (*device_a)->destroy();
    delete (*device_a);

    //TODO: delete from devices

    (*device_a) = nullptr;
}
} // namespace lx::gpu