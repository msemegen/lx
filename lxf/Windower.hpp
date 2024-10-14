#pragma once

/*
 *   Name: Windower.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <vector>

// lxf
#include <lxf/Canvas.hpp>
#include <lxf/common/bit.hpp>
#include <lxf/common/non_copyable.hpp>
#include <lxf/device.hpp>

namespace lxf {
class Windower : private common::non_copyable
{
public:
    template<typename Type_t> [[nodiscard]] Type_t* create(const device::Display& display_a, typename const Type_t::Descriptor& descriptor_a) = delete;
    template<typename Type_t> [[nodiscard]] bool destroy(Type_t** p_object_a) = delete;

private:

    std::vector<canvas::Fullscreen> fullscreen_canvases;
    std::vector<canvas::Windowed> windowed_canvases;
};

template<>
inline [[nodiscard]] canvas::Fullscreen* Windower::create<canvas::Fullscreen>(const device::Display& display_a, const canvas::Fullscreen::Descriptor& descriptor_a)
{
    this->fullscreen_canvases.emplace_back(display_a, descriptor_a);
    return &(this->fullscreen_canvases.back());
}
template<>
inline [[nodiscard]] canvas::Windowed* Windower::create<canvas::Windowed>(const device::Display& display_a, const canvas::Windowed::Descriptor& descriptor_a)
{
    this->windowed_canvases.emplace_back(display_a, descriptor_a);
    return &(this->windowed_canvases.back());
}

template<> inline [[nodiscard]] bool Windower::destroy<canvas::Fullscreen>(canvas::Fullscreen** p_object_a)
{
    std::vector<canvas::Fullscreen>::iterator itr =
        std::find(this->fullscreen_canvases.begin(), this->fullscreen_canvases.end(), (**p_object_a));

    if (this->fullscreen_canvases.end() != itr)
    {
        itr->destroy();
        this->fullscreen_canvases.erase(itr);

        (*p_object_a) = nullptr;

        return true;
    }

    return false;
}
template<> inline [[nodiscard]] bool Windower::destroy<canvas::Windowed>(canvas::Windowed** p_object_a)
{
    std::vector<canvas::Windowed>::iterator itr = std::find(this->windowed_canvases.begin(), this->windowed_canvases.end(), (**p_object_a));

    if (this->windowed_canvases.end() != itr)
    {
        itr->destroy();
        this->windowed_canvases.erase(itr);

        (*p_object_a) = nullptr;

        return true;
    }

    return false;
}
} // namespace lxf