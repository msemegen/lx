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
    template<typename Type> [[nodiscard]] Type create(const lx::devices::GPU& gpu_a,
                                                      const lx::Canvas<lx::canvas::framed>& canvas_a,
                                                      typename const Type::Properties& properties_a) = delete;
    template<typename Type> void destroy(lx::common::out<Type> obj) = delete;
};

template<> inline [[nodiscard]] lx::gpu::Device Context::create<lx::gpu::Device>(const lx::devices::GPU& gpu_a,
                                                                                 const lx::Canvas<lx::canvas::framed>& canvas_a,
                                                                                 const lx::gpu::Device::Properties& properties_a)
{
    return { gpu_a, canvas_a, properties_a };
}

template<> inline void Context::destroy<lx::gpu::Device>(lx::common::out<lx::gpu::Device> device_a)
{
    (*device_a).destroy();
}
} // namespace lx::gpu