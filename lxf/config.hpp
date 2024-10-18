#pragma once

/*
 *   Name: config.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// std
#include <cstdint>
#include <string_view>

// platform
#include <vulkan/vulkan.h>

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

        const static inline std::uint64_t max_supported_gpus = 10ull;
        const static inline std::uint64_t max_supported_displays = 10ull;
    };

    struct vulkan
    {
        struct validation_layer : private common::non_constructible
        {
            enum class Severity : std::uint32_t
            {
                none = 0xFFFFFFFFu,
                info = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
                warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
                error = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            };
            enum class Type : std::uint32_t
            {
                none = 0xFFFFFFFFu,
                general = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
                validation = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
                performance = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                device_address_binding = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
            };

            static bool enabled;
            static Severity severity;
            static Type type;
        };
        static common::Version version;
    };
};

constexpr config::vulkan::validation_layer::Severity operator|(config::vulkan::validation_layer::Severity left_a,
                                                               config::vulkan::validation_layer::Severity right_a)
{
    return static_cast<config::vulkan::validation_layer::Severity>(static_cast<std::uint64_t>(left_a) |
                                                                   static_cast<std::uint64_t>(right_a));
}
constexpr config::vulkan::validation_layer::Severity operator&(config::vulkan::validation_layer::Severity left_a,
                                                               config::vulkan::validation_layer::Severity right_a)
{
    return static_cast<config::vulkan::validation_layer::Severity>(static_cast<std::uint64_t>(left_a) &
                                                                   static_cast<std::uint64_t>(right_a));
}

constexpr config::vulkan::validation_layer::Type operator|(config::vulkan::validation_layer::Type left_a,
                                                           config::vulkan::validation_layer::Type right_a)
{
    return static_cast<config::vulkan::validation_layer::Type>(static_cast<std::uint64_t>(left_a) | static_cast<std::uint64_t>(right_a));
}
constexpr config::vulkan::validation_layer::Type operator&(config::vulkan::validation_layer::Type left_a,
                                                           config::vulkan::validation_layer::Type right_a)
{
    return static_cast<config::vulkan::validation_layer::Type>(static_cast<std::uint64_t>(left_a) & static_cast<std::uint64_t>(right_a));
}
} // namespace lxf