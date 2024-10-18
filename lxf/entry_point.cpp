#pragma once

/*
 *   Name: entry_point.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/entry_point.hpp>

// std
#include <array>
#include <cstdint>
#include <memory>

// lxf
#include <lxf/Windower.hpp>
#include <lxf/common/bit.hpp>
#include <lxf/common/non_constructible.hpp>
#include <lxf/config.hpp>
#include <lxf/device.hpp>
#include <lxf/loader.hpp>
#include <lxf/system.hpp>
#include <lxf/utils/config_parser.hpp>
#include <lxf/utils/logger.hpp>

VkInstance vk_instance;
VkDebugUtilsMessengerEXT vk_debug_messenger;

PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

// instance level functions
PFN_vkCreateDevice vkCreateDevice;
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
#if defined(VK_KHR_win32_surface)
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#if defined(VK_KHR_surface)
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
#endif
#if defined(VK_EXT_debug_utils)
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
#endif

namespace {
using namespace ::lxf;
using namespace ::lxf::common;
using namespace ::lxf::utils;

FILE* p_log_file = nullptr;

constexpr std::string_view module_name = "lxf:main";

logger::Severity get_log_severity_from_config(logger::Severity default_a, const std::vector<std::string_view>& config_a)
{
    logger::Severity ret = true == config_a.empty() ? default_a : logger::Severity::none;

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
            ret = ret | logger::Severity::info;
        }
    }

    return ret;
}

BOOL enum_monitors_handler(HMONITOR monitor_handle_a, HDC, LPRECT, LPARAM user_data_a)
{
    std::pair<std::array<device::Display, config::engine::max_supported_displays>*, std::size_t>* p_displays =
        std::bit_cast<std::pair<std::array<device::Display, config::engine::max_supported_displays>*, std::size_t>*>(user_data_a);

    MONITORINFOEX monitor_info;
    DEVMODE dev_mode;

    monitor_info.cbSize = sizeof(monitor_info);
    dev_mode.dmSize = sizeof(dev_mode);

    GetMonitorInfo(monitor_handle_a, &monitor_info);
    EnumDisplaySettings(monitor_info.szDevice, ENUM_CURRENT_SETTINGS, &dev_mode);

    if ((p_displays->second) < config::engine::max_supported_displays)
    {
        new (&p_displays->first[(p_displays->second)++]) device::Display(
            monitor_handle_a,
            0 == monitor_info.rcMonitor.left && 0 == monitor_info.rcMonitor.top,
            static_cast<std::uint8_t>(dev_mode.dmBitsPerPel),
            reinterpret_cast<const char*>(dev_mode.dmDeviceName),
            Rect<std::int32_t, std::uint32_t> { .position = { .x = monitor_info.rcMonitor.left, .y = monitor_info.rcMonitor.top },
                                                .size = { .w = static_cast<std::uint16_t>(monitor_info.rcMonitor.right),
                                                          .h = static_cast<std::uint16_t>(monitor_info.rcMonitor.bottom) } },
            Rect<std::int32_t, std::uint32_t> {
                .position = { .x = dev_mode.dmPosition.x, .y = dev_mode.dmPosition.y },
                .size = { .w = dev_mode.dmPosition.x + dev_mode.dmPelsWidth, .h = dev_mode.dmPosition.y + dev_mode.dmPelsHeight } });

        return TRUE;
    }

    return FALSE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT,
                                             VkDebugUtilsMessageTypeFlagsEXT,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void*)
{
    printf("VALIDATION LAYER: %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

} // namespace

namespace lxf::utils {
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
    switch (static_cast<Level>(static_cast<std::uint64_t>(level_a) & 0x1Full))
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
} // namespace lxf::utils

using namespace ::lxf;
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR p_cmd_line_a, _In_ int)
{
    using namespace ::lxf::utils;

    config_parser::allocate(p_cmd_line_a);

    // config reading
    {
        const std::vector<std::string_view> log_file = config_parser::get_value({ "lxf", "log", "file" });
        const std::vector<std::string_view> log_severity = config_parser::get_value({ "lxf", "log", "severity" });
        const std::vector<std::string_view> log_console_redirection = config_parser::get_value({ "lxf", "log", "console" });
        const std::vector<std::string_view> app_name = config_parser::get_value({ "lxf", "app", "name" });

        config::log::file_path = 1ull == log_file.size() ? log_file[0] : config::log::file_path;
        config::log::severity = get_log_severity_from_config(config::log::severity, log_severity);
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
    logger::write_line(logger::info, module_name, "Log started.");

    bool vk_loaded = loader::load();

    if (false == vk_loaded)
    {
        logger::write_line(logger::omg, module_name, "Vulkan not present in system");

        fclose(p_log_file);

        if (true == console_allocated)
        {
            printf("Press any key...\n");
            std::ignore = getc(stdin);
            FreeConsole();
        }

        return -1;
    }

    logger::write_line(logger::omg, module_name, "Vulkan successfully loaded");

    // instance layers enumeration an logging
    {
        std::uint32_t instance_layers_count = 0;
        vkEnumerateInstanceLayerProperties(&instance_layers_count, nullptr);
        std::unique_ptr<VkLayerProperties[]> instnace_layers_buffer = std::make_unique<VkLayerProperties[]>(instance_layers_count);
        vkEnumerateInstanceLayerProperties(&instance_layers_count, instnace_layers_buffer.get());

        logger::write_line(logger::Level::info, module_name, "Avaliable instance layers {} ", instance_layers_count);

        for (std::uint32_t i = 0; i < instance_layers_count; i++)
        {
            logger::write_line(logger::debug, module_name, "\t - {}", instnace_layers_buffer[i].layerName);
        }
    }

    // instance extensions enumeration and logging
    {
        std::uint32_t instance_extensions_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &instance_extensions_count, nullptr);
        std::unique_ptr<VkExtensionProperties[]> instnace_extensions_buffer =
            std::make_unique<VkExtensionProperties[]>(instance_extensions_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &instance_extensions_count, instnace_extensions_buffer.get());

        logger::write_line(logger::info, module_name, "Avaliable instance extensions {}", instance_extensions_count);

        for (std::uint32_t i = 0; i < instance_extensions_count; i++)
        {
            logger::write_line(logger::debug, module_name, "\t - {}", instnace_extensions_buffer[i].extensionName);
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
            logger::write_line(logger::omg, module_name, "No primary display adapter found.");
        }
    }

    std::vector<const char*> instance_extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
    std::vector<const char*> instance_layers;

    if (true == config::vulkan::validation_layer::enabled)
    {
        instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        instance_layers.push_back("VK_LAYER_KHRONOS_validation");
    }

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
                                                         .enabledLayerCount = static_cast<std::uint32_t>(instance_layers.size()),
                                                         .ppEnabledLayerNames = instance_layers.data(),
                                                         .enabledExtensionCount = static_cast<std::uint32_t>(instance_extensions.size()),
                                                         .ppEnabledExtensionNames = instance_extensions.data() };

    // instance creation
    {
        VkResult vk_result;
        vk_result = vkCreateInstance(&vk_instance_create_info, nullptr, &vk_instance);

        if (VK_SUCCESS != vk_result)
        {
            logger::write_line(
                logger::omg, module_name, "Cannot create Vulkan instance (error code: {})", static_cast<std::int32_t>(vk_result));
            return -1;
        }

        logger::write_line(logger::info,
                           module_name,
                           "Vulkan instance created (API version: {}.{}.{}).",
                           config::vulkan::version.get().major,
                           config::vulkan::version.get().minor,
                           config::vulkan::version.get().path);

        logger::write_line(logger::info, module_name, "Selected layers {}", vk_instance_create_info.enabledLayerCount);
        for (std::uint32_t layer_index = 0u; layer_index < vk_instance_create_info.enabledLayerCount; layer_index++)
        {
            logger::write_line(logger::info, module_name, "\t - {}", vk_instance_create_info.ppEnabledLayerNames[layer_index]);
        }

        logger::write_line(logger::info, module_name, "Selected extensions {}", vk_instance_create_info.enabledExtensionCount);
        for (std::uint32_t extension_index = 0u; extension_index < vk_instance_create_info.enabledExtensionCount; extension_index++)
        {
            logger::write_line(logger::info, module_name, "\t - {}", vk_instance_create_info.ppEnabledExtensionNames[extension_index]);
        }

        if (true == config::vulkan::validation_layer::enabled)
        {
            assert(config::vulkan::validation_layer::Type::none != config::vulkan::validation_layer::type &&
                   config::vulkan::validation_layer::Severity::none != config::vulkan::validation_layer::severity);

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .pNext = nullptr,
                .flags = 0x0u,
                .messageSeverity = static_cast<VkDebugUtilsMessageSeverityFlagsEXT>(config::vulkan::validation_layer::severity),
                .messageType = static_cast<VkDebugUtilsMessageTypeFlagsEXT>(config::vulkan::validation_layer::type),
                .pfnUserCallback = debugCallback,
                .pUserData = nullptr
            };

            vkCreateDebugUtilsMessengerEXT(vk_instance, &debugCreateInfo, nullptr, &vk_debug_messenger);
        }
    }

    // physical devices enumeration and logging
    std::array<device::GPU, config::engine::max_supported_gpus> gpus;
    std::uint32_t gpus_count = 0;
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
        logger::write_line(logger::info, module_name, "Avaliable display adapters: ");

        vkEnumeratePhysicalDevices(vk_instance, &gpus_count, nullptr);
        std::unique_ptr<VkPhysicalDevice[]> gpus_buffer = std::make_unique<VkPhysicalDevice[]>(gpus_count);
        vkEnumeratePhysicalDevices(vk_instance, &gpus_count, gpus_buffer.get());

        for (std::uint32_t gpu_index = 0; gpu_index < gpus_count && config::engine::max_supported_gpus; gpu_index++)
        {
            VkPhysicalDeviceProperties vk_device_properties;
            VkPhysicalDeviceFeatures vk_device_features;

            std::vector<std::string_view> device_extension_names;

            std::unique_ptr<VkExtensionProperties[]> vk_device_extensions_buffer;

            vkGetPhysicalDeviceProperties(gpus_buffer[gpu_index], &vk_device_properties);
            vkGetPhysicalDeviceFeatures(gpus_buffer[gpu_index], &vk_device_features);

            std::uint32_t device_extensions_count = 0u;
            vkEnumerateDeviceExtensionProperties(gpus_buffer[gpu_index], nullptr, &device_extensions_count, nullptr);

            if (0u != device_extensions_count)
            {
                vk_device_extensions_buffer = std::make_unique<VkExtensionProperties[]>(device_extensions_count);
                vkEnumerateDeviceExtensionProperties(
                    gpus_buffer[gpu_index], nullptr, &device_extensions_count, vk_device_extensions_buffer.get());

                device_extension_names.reserve(device_extensions_count);

                for (std::uint32_t ex_index = 0; ex_index < device_extensions_count; ex_index++)
                {
                    device_extension_names.emplace_back(vk_device_extensions_buffer[ex_index].extensionName);
                }
            }

            bool is_primary = vk_device_properties.deviceName == primary_display_device_name;

            std::uint32_t queue_family_property_count = 0u;
            vkGetPhysicalDeviceQueueFamilyProperties(gpus_buffer[gpu_index], &queue_family_property_count, nullptr);

            std::unique_ptr<VkQueueFamilyProperties[]> vk_queue_family_properties_buffer =
                std::make_unique<VkQueueFamilyProperties[]>(queue_family_property_count);
            vkGetPhysicalDeviceQueueFamilyProperties(
                gpus_buffer[gpu_index], &queue_family_property_count, vk_queue_family_properties_buffer.get());

            logger::write_line(logger::info,
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

                logger::write_line(logger::info,
                                   module_name,
                                   "\t\t Queue family: {}, count: {}",
                                   buff,
                                   vk_queue_family_properties_buffer[qf_index].queueCount);
            }

            logger::write_line(logger::info, module_name, "\t\t Extensions {}", device_extension_names.size());
            for (const auto& ext_name : device_extension_names)
            {
                logger::write_line(logger::Level::debug, module_name, "\t\t\t - {}", &(ext_name[0]));
            }

            // placement new in array element
            new (&(gpus[gpu_index])) device::GPU(gpus_buffer[gpu_index],
                                                 is_primary,
                                                 std::span { vk_queue_family_properties_buffer.get(), queue_family_property_count },
                                                 vk_device_properties.deviceName,
                                                 device_extension_names);
        }
    }

    // displays enumerating an logging
    std::array<device::Display, config::engine::max_supported_displays> displays;
    std::size_t displays_count = 0;
    {
        std::pair<std::array<device::Display, config::engine::max_supported_displays>*, std::size_t> enum_callback_data { &displays, 0u };
        EnumDisplayMonitors(nullptr, nullptr, enum_monitors_handler, reinterpret_cast<LPARAM>(&enum_callback_data));

        displays_count = enum_callback_data.second;
        logger::write_line(logger::info, module_name, "Avaliable displays {}", displays_count);

        for (std::size_t display_index = 0; display_index < displays_count; display_index++)
        {
            device::Display::Properties display_properties = displays[display_index].get_properties();

            logger::write_line(logger::debug,
                               module_name,
                               "\t - {}{}, bpp: {}, logical resolution: {}x{}, physical resolution: {}x{}",
                               device::Display::Kind::primary == display_properties.kind ? "*" : "",
                               display_properties.name,
                               display_properties.bits_per_pixel,
                               display_properties.logical_rect.size.w,
                               display_properties.logical_rect.size.h,
                               display_properties.physical_rect.size.w,
                               display_properties.physical_rect.size.h);
        }
    }

    Windower windower;

    entry_point(p_cmd_line_a, std::span<device::GPU> { &(gpus[0]), gpus_count }, { &displays[0], displays_count }, &windower);

    if (true == config::vulkan::validation_layer::enabled)
    {
        vkDestroyDebugUtilsMessengerEXT(vk_instance, vk_debug_messenger, nullptr);
    }

    vkDestroyInstance(vk_instance, nullptr);
    logger::write_line(logger::info, module_name, "Vulkan instance destroyed.");

    loader::close();
    logger::write_line(logger::info, module_name, "Vulkan unloaded.");

    logger::write_line(logger::info, module_name, "Log ended.");

    fclose(p_log_file);

    if (true == console_allocated)
    {
        printf("Press any key...\n");
        std::ignore = getc(stdin);
        FreeConsole();
    }

    return 0;
}