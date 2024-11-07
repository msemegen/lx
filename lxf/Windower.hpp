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

namespace lxf {
class Windower : private common::non_copyable
{
public:
    template<typename Type>
    [[nodiscard]] Type* create(common::Rect<std::int32_t, std::uint32_t>, typename const Type::Properties&) = delete;
    template<typename Type> [[nodiscard]] bool destroy(Type**) = delete;

private:
    std::vector<canvas::Fullscreen> fullscreen_canvases;
    std::vector<canvas::Windowed> windowed_canvases;
};

template<> inline [[nodiscard]] canvas::Fullscreen* Windower::create<canvas::Fullscreen>(common::Rect<std::int32_t, std::uint32_t> screen,
                                                                                         const canvas::Fullscreen::Properties& properties)
{
    this->fullscreen_canvases.emplace_back(screen, properties);
    return &(this->fullscreen_canvases.back());
}
template<> inline [[nodiscard]] canvas::Windowed* Windower::create<canvas::Windowed>(common::Rect<std::int32_t, std::uint32_t> screen,
                                                                                     const canvas::Windowed::Properties& properties)
{
    this->windowed_canvases.emplace_back(screen, properties);
    return &(this->windowed_canvases.back());
}

template<> inline [[nodiscard]] bool Windower::destroy<canvas::Fullscreen>(canvas::Fullscreen** object)
{
    std::vector<canvas::Fullscreen>::iterator itr =
        std::find(this->fullscreen_canvases.begin(), this->fullscreen_canvases.end(), (**object));

    if (this->fullscreen_canvases.end() != itr)
    {
        itr->destroy();
        this->fullscreen_canvases.erase(itr);

        (*object) = nullptr;

        return true;
    }

    return false;
}
template<> inline [[nodiscard]] bool Windower::destroy<canvas::Windowed>(canvas::Windowed** object)
{
    std::vector<canvas::Windowed>::iterator itr = std::find(this->windowed_canvases.begin(), this->windowed_canvases.end(), (**object));

    if (this->windowed_canvases.end() != itr)
    {
        itr->destroy();
        this->windowed_canvases.erase(itr);

        (*object) = nullptr;

        return true;
    }

    return false;
}
} // namespace lxf