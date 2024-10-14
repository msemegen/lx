#pragma once

/*
 *   Name: config.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <string_view>

// lxf
#include <lxf/common/Version.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/utils/logger.hpp>

namespace lxf {
struct config : private common::non_constructible
{
    struct log : private common::non_constructible
    {
        static std::string_view file_path;
        static utils::logger::Severity severity;
        static bool console;
    };

    struct app : private common::non_constructible
    {
        static std::string_view name;
        static common::Version version;
    };

    struct engine : private common::non_constructible
    {
        static std::string_view name;
        static common::Version version;
    };

    struct vulkan
    {
        static common::Version version;
    };
};
} // namespace lxf