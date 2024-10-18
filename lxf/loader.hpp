#pragma once

/*
 *   Name: loader.hpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// platform
#include <vulkan/vulkan.h>

// lfx
#include <lxf/common/non_constructible.hpp>

extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

// instance level functions
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkDestroyInstance vkDestroyInstance;
extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
#if defined(VK_KHR_win32_surface)
extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#if defined(VK_KHR_surface)
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
#endif
#if defined(VK_EXT_debug_utils)
extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
#endif

namespace lxf {
struct loader : private common::non_constructible
{
private:
public:
    static bool load()
    {
        vk_handle = LoadLibrary("vulkan-1.dll");
        if (nullptr != vk_handle)
        {
            vk_get_instance_proc_addr =
                reinterpret_cast<decltype(vk_get_instance_proc_addr)>(GetProcAddress(vk_handle, "vkGetInstanceProcAddr"));
            vk_create_instance = reinterpret_cast<decltype(vk_create_instance)>(GetProcAddress(vk_handle, "vkCreateInstance"));

            vkEnumerateInstanceExtensionProperties = reinterpret_cast<decltype(vkEnumerateInstanceExtensionProperties)>(
                GetProcAddress(vk_handle, "vkEnumerateInstanceExtensionProperties"));
            vkEnumerateInstanceLayerProperties = reinterpret_cast<decltype(vkEnumerateInstanceLayerProperties)>(
                GetProcAddress(vk_handle, "vkEnumerateInstanceLayerProperties"));

            return true;
        }

        return false;
    }

    static void close()
    {
        if (nullptr != vk_handle)
        {
            FreeLibrary(vk_handle);
        }
    }

private:
    static inline HMODULE vk_handle = nullptr;

    static inline PFN_vkGetInstanceProcAddr vk_get_instance_proc_addr = nullptr;
    static inline PFN_vkCreateInstance vk_create_instance = nullptr;

public:
    friend VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                                                           const VkAllocationCallbacks* pAllocator,
                                                           VkInstance* pInstance);
};

inline VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                                                       const VkAllocationCallbacks* pAllocator,
                                                       VkInstance* pInstance)
{
    assert(nullptr != loader::vk_get_instance_proc_addr && nullptr != loader::vk_create_instance);

    VkResult res = loader::vk_create_instance(pCreateInfo, pAllocator, pInstance);

    if (VK_SUCCESS == res)
    {
        vkCreateDevice = reinterpret_cast<decltype(vkCreateDevice)>(loader::vk_get_instance_proc_addr(*pInstance, "vkCreateDevice"));
        vkDestroyDevice = reinterpret_cast<decltype(vkDestroyDevice)>(loader::vk_get_instance_proc_addr(*pInstance, "vkDestroyDevice"));
        vkDestroyInstance =
            reinterpret_cast<decltype(vkDestroyInstance)>(loader::vk_get_instance_proc_addr(*pInstance, "vkDestroyInstance"));
        vkGetPhysicalDeviceProperties = reinterpret_cast<decltype(vkGetPhysicalDeviceProperties)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceProperties"));
        vkGetPhysicalDeviceFeatures = reinterpret_cast<decltype(vkGetPhysicalDeviceFeatures)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceFeatures"));
        vkEnumeratePhysicalDevices = reinterpret_cast<decltype(vkEnumeratePhysicalDevices)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkEnumeratePhysicalDevices"));
        vkEnumerateDeviceExtensionProperties = reinterpret_cast<decltype(vkEnumerateDeviceExtensionProperties)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkEnumerateDeviceExtensionProperties"));
        vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<decltype(vkGetPhysicalDeviceQueueFamilyProperties)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties"));

#if defined(VK_KHR_win32_surface)
        vkCreateWin32SurfaceKHR =
            reinterpret_cast<decltype(vkCreateWin32SurfaceKHR)>(loader::vk_get_instance_proc_addr(*pInstance, "vkCreateWin32SurfaceKHR"));
        vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceWin32PresentationSupportKHR)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
#endif
#if defined(VK_KHR_surface)
        vkDestroySurfaceKHR =
            reinterpret_cast<decltype(vkDestroySurfaceKHR)>(loader::vk_get_instance_proc_addr(*pInstance, "vkDestroySurfaceKHR"));
#endif
#if defined(VK_EXT_debug_utils)
        vkCreateDebugUtilsMessengerEXT = reinterpret_cast<decltype(vkCreateDebugUtilsMessengerEXT)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkCreateDebugUtilsMessengerEXT"));
        vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<decltype(vkDestroyDebugUtilsMessengerEXT)>(
            loader::vk_get_instance_proc_addr(*pInstance, "vkDestroyDebugUtilsMessengerEXT"));
#endif
    }
    return res;
}
} // namespace lxf