#pragma once

/*
 *   Name: Scene.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <vector>

// lxf
#include <lxf/common/non_copyable.hpp>

namespace lxe {
class Scene : private lxf::common::non_copyable
{
public:
    class Layer : private lxf::common::non_copyable
    {
    public:
        class Actor
        {
        };

        Actor* add_actor() {}

    private:
        std::vector<Actor> actors;
    };

    Layer* add_layer() {}

private:
    std::vector<Layer*> layers;
};
} // namespace lxe