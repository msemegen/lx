/*
 *   Name: vulkan.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lxf/loader/vulkan.hpp>

// std
#include <cassert>

namespace {
HMODULE vk_handle = nullptr;

PFN_vkGetInstanceProcAddr vk_get_instance_proc_addr = nullptr;
PFN_vkGetDeviceProcAddr vk_get_device_proc_addr = nullptr;

} // namespace

PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

// instance level functions
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

// device level functions
PFN_vkGetDeviceQueue vkGetDeviceQueue;

namespace lxf::loader {
bool vulkan::load()
{
    vk_handle = LoadLibrary("vulkan-1.dll");
    if (nullptr != vk_handle)
    {
        vk_get_instance_proc_addr =
            reinterpret_cast<decltype(vk_get_instance_proc_addr)>(GetProcAddress(vk_handle, "vkGetInstanceProcAddr"));
        vk_get_device_proc_addr = reinterpret_cast<decltype(vk_get_device_proc_addr)>(GetProcAddress(vk_handle, "vkGetDeviceProcAddr"));

        vkEnumerateInstanceExtensionProperties = reinterpret_cast<decltype(vkEnumerateInstanceExtensionProperties)>(
            GetProcAddress(vk_handle, "vkEnumerateInstanceExtensionProperties"));
        vkEnumerateInstanceLayerProperties =
            reinterpret_cast<decltype(vkEnumerateInstanceLayerProperties)>(GetProcAddress(vk_handle, "vkEnumerateInstanceLayerProperties"));

        return true;
    }

    return false;
}
void vulkan::release()
{
    assert(nullptr != vk_handle);
    FreeLibrary(vk_handle);

    vk_handle = nullptr;
}

extern "C" {
VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                                                const VkAllocationCallbacks* pAllocator,
                                                VkInstance* pInstance)
{
    assert(nullptr != vk_handle);

    PFN_vkCreateInstance vk_create_instance = reinterpret_cast<decltype(vk_create_instance)>(GetProcAddress(vk_handle, "vkCreateInstance"));
    VkResult ret = vk_create_instance(pCreateInfo, pAllocator, pInstance);

    if (VK_SUCCESS == ret)
    {
        vkDestroyInstance = reinterpret_cast<decltype(vkDestroyInstance)>(vk_get_instance_proc_addr(*pInstance, "vkDestroyInstance"));
        vkGetPhysicalDeviceProperties = reinterpret_cast<decltype(vkGetPhysicalDeviceProperties)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceProperties"));
        vkGetPhysicalDeviceFeatures =
            reinterpret_cast<decltype(vkGetPhysicalDeviceFeatures)>(vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceFeatures"));
        vkEnumeratePhysicalDevices =
            reinterpret_cast<decltype(vkEnumeratePhysicalDevices)>(vk_get_instance_proc_addr(*pInstance, "vkEnumeratePhysicalDevices"));
        vkEnumerateDeviceExtensionProperties = reinterpret_cast<decltype(vkEnumerateDeviceExtensionProperties)>(
            vk_get_instance_proc_addr(*pInstance, "vkEnumerateDeviceExtensionProperties"));
        vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<decltype(vkGetPhysicalDeviceQueueFamilyProperties)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties"));

#if defined(VK_KHR_win32_surface)
        vkCreateWin32SurfaceKHR =
            reinterpret_cast<decltype(vkCreateWin32SurfaceKHR)>(vk_get_instance_proc_addr(*pInstance, "vkCreateWin32SurfaceKHR"));
        vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceWin32PresentationSupportKHR)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
#endif
#if defined(VK_KHR_surface)
        vkDestroySurfaceKHR = reinterpret_cast<decltype(vkDestroySurfaceKHR)>(vk_get_instance_proc_addr(*pInstance, "vkDestroySurfaceKHR"));
#endif
#if defined(VK_EXT_debug_utils)
        vkCreateDebugUtilsMessengerEXT = reinterpret_cast<decltype(vkCreateDebugUtilsMessengerEXT)>(
            vk_get_instance_proc_addr(*pInstance, "vkCreateDebugUtilsMessengerEXT"));
        vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<decltype(vkDestroyDebugUtilsMessengerEXT)>(
            vk_get_instance_proc_addr(*pInstance, "vkDestroyDebugUtilsMessengerEXT"));
#endif
    }

    return ret;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice,
                                              const VkDeviceCreateInfo* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDevice* pDevice)
{
    PFN_vkCreateDevice vk_create_device = reinterpret_cast<decltype(vk_create_device)>(GetProcAddress(vk_handle, "vkCreateDevice"));
    VkResult ret = vk_create_device(physicalDevice, pCreateInfo, pAllocator, pDevice);

    if (VK_SUCCESS == ret)
    {
        vkDestroyDevice = reinterpret_cast<decltype(vkDestroyDevice)>(vk_get_device_proc_addr(*pDevice, "vkDestroyDevice"));
        vkGetDeviceQueue = reinterpret_cast<decltype(vkGetDeviceQueue)>(vk_get_device_proc_addr(*pDevice, "vkGetDeviceQueue"));
    }

    return ret;
}
}
} // namespace lxf::loader