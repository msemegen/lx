#pragma once

/*
 *   Name: entry_point.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/entry_point.hpp>

// std
#include <memory>

// platform
#include <Windows.h>
#include <vulkan/vulkan.h>

// lxf
#include <lxf/Windower.hpp>
#include <lxf/common/bit.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/config.hpp>
#include <lxf/device.hpp>
#include <lxf/system.hpp>
#include <lxf/utils/config_parser.hpp>
#include <lxf/utils/logger.hpp>

VkInstance vk_instance;

namespace {
using namespace ::lxf;
using namespace ::lxf::common;
using namespace ::lxf::utils;

FILE* p_log_file = nullptr;

constexpr std::string_view module_name = "lxf:main";

logger::Severity get_log_severity_from_config(const std::vector<std::string_view>& config_a)
{
    logger::Severity ret = logger::Severity::none;

    for (const std::string_view& s : config_a)
    {
        if ("inf" == s)
        {
            ret = ret | logger::Severity::info;
        }

        if ("wrn" == s)
        {
            ret = ret | logger::Severity::warning;
        }

        if ("err" == s)
        {
            ret = ret | logger::Severity::error;
        }

        if ("omg" == s)
        {
            ret = ret | logger::Severity::omg;
        }

        if ("dbg" == s)
        {
            ret = ret | logger::Severity::debug;
        }
    }

    return ret;
}

BOOL enum_monitors_handler(HMONITOR monitor_handle_a, HDC, LPRECT, LPARAM user_data_a)
{
    std::vector<device::Display>* p_displays = reinterpret_cast<std::vector<device::Display>*>(user_data_a);

    MONITORINFOEX monitor_info;
    DEVMODE dev_mode;

    monitor_info.cbSize = sizeof(monitor_info);
    dev_mode.dmSize = sizeof(dev_mode);

    GetMonitorInfo(monitor_handle_a, &monitor_info);
    EnumDisplaySettings(monitor_info.szDevice, ENUM_CURRENT_SETTINGS, &dev_mode);

    p_displays->emplace_back(monitor_handle_a,
                             static_cast<Uint8>(dev_mode.dmBitsPerPel),
                             reinterpret_cast<const char*>(dev_mode.dmDeviceName),
                             Rect<Int32, Uint32> { .position = { .x = monitor_info.rcMonitor.left, .y = monitor_info.rcMonitor.top },
                                                   .size = { .w = static_cast<Uint16>(monitor_info.rcMonitor.right),
                                                             .h = static_cast<Uint16>(monitor_info.rcMonitor.bottom) } },
                             Rect<Int32, Uint32> { .position = { .x = dev_mode.dmPosition.x, .y = dev_mode.dmPosition.y },
                                                   .size = { .w = dev_mode.dmPosition.x + dev_mode.dmPelsWidth,
                                                             .h = dev_mode.dmPosition.y + dev_mode.dmPelsHeight } });

    return TRUE;
}

} // namespace

namespace lxf {
namespace utils {
void logger::timestamp(std::time_t timestamp_a)
{
    std::tm tim;
    /*std::*/ localtime_s(&tim, &timestamp_a);

    std::fprintf(
        p_log_file, "[%02d:%02d:%02d %02d-%02d-%d]", tim.tm_hour, tim.tm_min, tim.tm_sec, tim.tm_mday, tim.tm_mon + 1, tim.tm_year + 1900);
    std::fflush(p_log_file);

    if (true == is_console_output())
    {
        std::fprintf(
            stdout, "[%02d:%02d:%02d %02d-%02d-%d]", tim.tm_hour, tim.tm_min, tim.tm_sec, tim.tm_mday, tim.tm_mon + 1, tim.tm_year + 1900);
    }
}

void logger::log(Level level_a, std::string_view module_name_a, const std::string& log_a)
{
    std::fprintf(p_log_file, "[%s][%s] %s", module_name_a.data(), to_string(level_a), log_a.c_str());
    std::fflush(p_log_file);

    if (true == is_console_output())
    {
        std::fprintf(stdout, "[%s][%s] %s", module_name_a.data(), to_string(level_a), log_a.c_str());
    }
}

void logger::nl()
{
    std::fprintf(p_log_file, "\n");
    std::fflush(p_log_file);

    if (true == is_console_output())
    {
        std::fprintf(stdout, "\n");
    }
}

bool logger::is_open()
{
    return nullptr != p_log_file;
}

const char* logger::to_string(Level level_a)
{
    switch (static_cast<Level>(static_cast<common::Uint64>(level_a) & 0xFull))
    {
        case Level::debug:
            return "dbg";
        case Level::info:
            return "inf";
        case Level::warning:
            return "wrn";
        case Level::error:
            return "err";
        case Level::omg:
            return "omg";
    }
    return "";
}
} // namespace utils
} // namespace lxf

using namespace ::lxf;
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR p_cmd_line_a, _In_ int)
{
    using namespace ::lxf::utils;

    config_parser::allocate(p_cmd_line_a);

    const Version v = { VK_MAKE_API_VERSION(0u, 3u, 1u, 1u) };

    // config reading
    {
        const std::vector<std::string_view> log_file = config_parser::get_value({ "lxf", "log", "file" });
        const std::vector<std::string_view> log_severity = config_parser::get_value({ "lxf", "log", "severity" });
        const std::vector<std::string_view> log_console_redirection = config_parser::get_value({ "lxf", "log", "console" });
        const std::vector<std::string_view> app_name = config_parser::get_value({ "lxf", "app", "name" });

        config::log::file_path = 1ull == log_file.size() ? log_file[0] : config::log::file_path;
        config::log::severity = get_log_severity_from_config(log_severity);
        config::log::console = 1ull == log_console_redirection.size() ? "true" == log_console_redirection[0] : config::log::console;
    }

    /*std::*/ errno_t log_file_err = /*std::*/ fopen_s(&p_log_file, config::log::file_path.data(), "w");

    if (nullptr == p_log_file && 0 != log_file_err)
    {
        const std::string message = std::format("Cannot create log file: {}", config::log::file_path);
        system::message_box::show("Log file error", message, system::message_box::Icon::error, system::message_box::Button::ok);
        return -1;
    }

    bool console_allocated = false;
    if (true == config::log::console)
    {
        console_allocated = static_cast<bool>(AllocConsole());

        if (true == console_allocated)
        {
            FILE* fp1 = nullptr;
            FILE* fp2 = nullptr;
            FILE* fp3 = nullptr;

            freopen_s(&fp1, "CONIN$", "r", stdin);
            freopen_s(&fp2, "CONOUT$", "w", stdout);
            freopen_s(&fp3, "CONOUT$", "w", stderr);

            logger::set_console_output(true);
        }
    }

    logger::set_severity(config::log::severity);
    logger::write_line(logger::Level::info | logger::force, module_name, "Log started.");

    // instance layers enumeration an logging
    {
        logger::write_line(logger::Level::info | logger::force, module_name, "Avaliable instance layers: ");

        std::uint32_t instance_layers_count = 0;
        vkEnumerateInstanceLayerProperties(&instance_layers_count, nullptr);
        std::unique_ptr<VkLayerProperties[]> instnace_layers_buffer = std::make_unique<VkLayerProperties[]>(instance_layers_count);
        vkEnumerateInstanceLayerProperties(&instance_layers_count, instnace_layers_buffer.get());

        for (std::uint32_t i = 0; i < instance_layers_count; i++)
        {
            logger::write_line(logger::Level::info | logger::force, module_name, "\t - {}", instnace_layers_buffer[i].layerName);
        }
    }

    // instance extensions enumeration and logging
    {
        logger::write_line(logger::Level::info | logger::force, module_name, "Avaliable instance extensions: ");

        std::uint32_t instance_extensions_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &instance_extensions_count, nullptr);
        std::unique_ptr<VkExtensionProperties[]> instnace_extensions_buffer =
            std::make_unique<VkExtensionProperties[]>(instance_extensions_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &instance_extensions_count, instnace_extensions_buffer.get());

        for (std::uint32_t i = 0; i < instance_extensions_count; i++)
        {
            logger::write_line(logger::Level::info | logger::force, module_name, "\t - {}", instnace_extensions_buffer[i].extensionName);
        }
    }

    // find primary display adapter
    std::string primary_display_device_name;
    {
        DISPLAY_DEVICE win32_display_device;
        bool found = false;
        win32_display_device.cb = sizeof(win32_display_device);

        while (false == found && TRUE == EnumDisplayDevices(nullptr, 0, &win32_display_device, 0u))
        {
            found = DISPLAY_DEVICE_PRIMARY_DEVICE == (win32_display_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE);

            if (true == found)
            {
                primary_display_device_name = win32_display_device.DeviceString;
            }
        }

        if (false == found)
        {
            logger::write_line(logger::Level::omg | logger::force, module_name, "No primary display adapter found.");
            return -1;
        }
    }

    std::vector<const char*> instance_extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

    const VkApplicationInfo application_info { .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                               .pNext = nullptr,
                                               .pApplicationName = config::app::name.data(),
                                               .applicationVersion = config::app::version,
                                               .pEngineName = config::engine::name.data(),
                                               .engineVersion = config::engine::version,
                                               .apiVersion = config::vulkan::version };
    const VkInstanceCreateInfo vk_instance_create_info { .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                                         .pNext = nullptr,
                                                         .flags = 0x0u,
                                                         .pApplicationInfo = &application_info,
                                                         .enabledLayerCount = 0u, /* + gathered from config*/
                                                         .ppEnabledLayerNames = nullptr,
                                                         .enabledExtensionCount = 2u /* + gathered from config*/,
                                                         .ppEnabledExtensionNames = instance_extensions.data() };

    // instance creation
    {
        VkResult vk_result;
        vk_result = vkCreateInstance(&vk_instance_create_info, nullptr, &vk_instance);

        if (VK_SUCCESS != vk_result)
        {
            logger::write_line(logger::Level::omg | logger::force,
                               module_name,
                               "Cannot create Vulkan instance (error code: {})",
                               static_cast<std::int32_t>(vk_result));
            return -1;
        }

        logger::write_line(logger::Level::info | logger::force,
                           module_name,
                           "Vulkan instance created (API version: {}.{}.{}).",
                           config::vulkan::version.get().major,
                           config::vulkan::version.get().minor,
                           config::vulkan::version.get().path);

        if (false == instance_extensions.empty())
        {
            logger::write_line(logger::Level::info | logger::force, module_name, "Selected extensions:");

            for (const char* p_extension_name : instance_extensions)
            {
                logger::write_line(logger::Level::info | logger::force, module_name, "\t - {}", p_extension_name);
            }
        }
    }

    // physical devices enumeration and logging
    std::vector<device::GPU> gpus;
    {
        auto to_string = [](VkPhysicalDeviceType type_a) -> std::string_view {
            switch (type_a)
            {
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    return " CPU";

                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    return "DGPU";

                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    return "IGPU";

                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    return "VGPU";

                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    return "OTHE";
            }

            return "";
        };
        logger::write_line(logger::Level::info | logger::force, module_name, "Avaliable display adapters: ");

        std::uint32_t gpus_count = 0;
        vkEnumeratePhysicalDevices(vk_instance, &gpus_count, nullptr);
        std::unique_ptr<VkPhysicalDevice[]> gpus_buffer = std::make_unique<VkPhysicalDevice[]>(gpus_count);
        vkEnumeratePhysicalDevices(vk_instance, &gpus_count, gpus_buffer.get());

        for (std::uint32_t gpu_index = 0; gpu_index < gpus_count; gpu_index++)
        {
            VkPhysicalDeviceProperties vk_device_properties;
            VkPhysicalDeviceFeatures vk_device_features;

            std::unique_ptr<VkExtensionProperties[]> vk_device_extensions_buffer;

            vkGetPhysicalDeviceProperties(gpus_buffer[gpu_index], &vk_device_properties);
            vkGetPhysicalDeviceFeatures(gpus_buffer[gpu_index], &vk_device_features);
            //vkEnumerateDeviceExtensionProperties();

            bool is_primary = vk_device_properties.deviceName == primary_display_device_name;

            std::uint32_t queue_family_property_count = 0u;
            vkGetPhysicalDeviceQueueFamilyProperties(gpus_buffer[gpu_index], &queue_family_property_count, nullptr);

            std::unique_ptr<VkQueueFamilyProperties[]> vk_queue_family_properties_buffer =
                std::make_unique<VkQueueFamilyProperties[]>(queue_family_property_count);
            vkGetPhysicalDeviceQueueFamilyProperties(
                gpus_buffer[gpu_index], &queue_family_property_count, vk_queue_family_properties_buffer.get());

            logger::write_line(logger::Level::info | logger::force,
                               module_name,
                               "\t - [{}{}] - {}",
                               true == is_primary ? "*" : " ",
                               to_string(vk_device_properties.deviceType),
                               vk_device_properties.deviceName);
            for (std::uint32_t qf_index = 0; qf_index < queue_family_property_count; qf_index++)
            {
                char buff[30];
                std::size_t pos = 0u;

                auto put = [&](char c1_a, char c2_a = '\0') {
                    if (pos > 0u)
                    {
                        buff[pos++] = '|';
                    }

                    buff[pos++] = c1_a;
                    if ('\0' != c2_a)
                    {
                        buff[pos++] = c2_a;
                    }
                };

                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_GRAPHICS_BIT)))
                {
                    put('G');
                }
                if (true ==
                    bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags, static_cast<std::uint32_t>(VK_QUEUE_COMPUTE_BIT)))
                {
                    put('C');
                }
                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_TRANSFER_BIT)))
                {
                    put('T');
                }
                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_SPARSE_BINDING_BIT)))
                {
                    put('S', 'B');
                }
                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_PROTECTED_BIT)))
                {
                    put('P');
                }
                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_VIDEO_DECODE_BIT_KHR)))
                {
                    put('V', 'D');
                }
                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_VIDEO_ENCODE_BIT_KHR)))
                {
                    put('V', 'E');
                }
                if (true == bit::flag::is(vk_queue_family_properties_buffer[qf_index].queueFlags,
                                          static_cast<std::uint32_t>(VK_QUEUE_OPTICAL_FLOW_BIT_NV)))
                {
                    put('O', 'V');
                }

                buff[pos] = '\0';

                logger::write_line(logger::Level::info | logger::force,
                                   module_name,
                                   "\t\t Queue family: {}, count: {}",
                                   buff,
                                   vk_queue_family_properties_buffer[qf_index].queueCount);
            }

            gpus.emplace_back(gpus_buffer[gpu_index],
                              vk_device_properties.deviceType,
                              vk_device_properties.limits,
                              vk_device_features,
                              std::span { vk_queue_family_properties_buffer.get(), queue_family_property_count },
                              is_primary,
                              vk_device_properties.deviceName);
        }
    }

    // displays enumerating an logging
    std::vector<device::Display> displays;
    {
        logger::write_line(logger::Level::info | logger::force, module_name, "Avaliable displays: ");

        EnumDisplayMonitors(nullptr, nullptr, enum_monitors_handler, reinterpret_cast<LPARAM>(&displays));

        for (const device::Display& display : displays)
        {
            logger::write_line(logger::Level::info | logger::force,
                               module_name,
                               "\t - {}, bpp: {}, logical resolution: {}x{}, physical resolution: {}x{}",
                               display.get_name(),
                               display.get_bits_per_pixel(),
                               display.get_logical_rect().size.w,
                               display.get_logical_rect().size.h,
                               display.get_physical_rect().size.w,
                               display.get_physical_rect().size.h);
        }
    }

    Windower windower;

    entry_point(p_cmd_line_a, gpus, displays, &windower);

    vkDestroyInstance(vk_instance, nullptr);
    logger::write_line(logger::Level::info | logger::force, module_name, "Vulkan instance destroyed.");
    logger::write_line(logger::Level::info | logger::force, module_name, "Log ended.");
    fclose(p_log_file);

    if (true == console_allocated)
    {
        std::ignore = getc(stdin);
        FreeConsole();
    }

    return 0;
}