/*
 *   Name: vulkan.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lx/gpu/loader/vulkan.hpp>

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
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
#if defined(VK_KHR_surface)
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
#endif
#if defined(VK_KHR_win32_surface)
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#if defined(VK_EXT_debug_utils)
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
#endif

// device level functions
#if defined(VK_VERSION_1_0)
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkGetDeviceQueue vkGetDeviceQueue;
PFN_vkCreateShaderModule vkCreateShaderModule;
PFN_vkDestroyShaderModule vkDestroyShaderModule;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkDestroyPipeline vkDestroyPipeline;

#endif
#if defined(VK_KHR_swapchain)
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
#endif

namespace lx::gpu::loader {
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
#if defined(VK_KHR_surface)
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
        vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceSurfaceFormatsKHR)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
        vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceSurfacePresentModesKHR)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
        vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceSurfaceSupportKHR)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
        vkDestroySurfaceKHR = reinterpret_cast<decltype(vkDestroySurfaceKHR)>(vk_get_instance_proc_addr(*pInstance, "vkDestroySurfaceKHR"));
#endif
#if defined(VK_KHR_win32_surface)
        vkCreateWin32SurfaceKHR =
            reinterpret_cast<decltype(vkCreateWin32SurfaceKHR)>(vk_get_instance_proc_addr(*pInstance, "vkCreateWin32SurfaceKHR"));
        vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<decltype(vkGetPhysicalDeviceWin32PresentationSupportKHR)>(
            vk_get_instance_proc_addr(*pInstance, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
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
#if defined(VK_VERSION_1_0)
        vkDestroyDevice = reinterpret_cast<decltype(vkDestroyDevice)>(vk_get_device_proc_addr(*pDevice, "vkDestroyDevice"));
        vkGetDeviceQueue = reinterpret_cast<decltype(vkGetDeviceQueue)>(vk_get_device_proc_addr(*pDevice, "vkGetDeviceQueue"));
        vkCreateShaderModule = reinterpret_cast<decltype(vkCreateShaderModule)>(vk_get_device_proc_addr(*pDevice, "vkCreateShaderModule"));
        vkDestroyShaderModule =
            reinterpret_cast<decltype(vkDestroyShaderModule)>(vk_get_device_proc_addr(*pDevice, "vkDestroyShaderModule"));
        vkCreateGraphicsPipelines =
            reinterpret_cast<decltype(vkCreateGraphicsPipelines)>(vk_get_device_proc_addr(*pDevice, "vkCreateGraphicsPipelines"));
        vkDestroyPipeline = reinterpret_cast<decltype(vkDestroyPipeline)>(vk_get_device_proc_addr(*pDevice, "vkDestroyPipeline"));
#endif
#if defined(VK_KHR_swapchain)
        vkCreateSwapchainKHR = reinterpret_cast<decltype(vkCreateSwapchainKHR)>(vk_get_device_proc_addr(*pDevice, "vkCreateSwapchainKHR"));
        vkGetSwapchainImagesKHR =
            reinterpret_cast<decltype(vkGetSwapchainImagesKHR)>(vk_get_device_proc_addr(*pDevice, "vkGetSwapchainImagesKHR"));
        vkDestroySwapchainKHR =
            reinterpret_cast<decltype(vkDestroySwapchainKHR)>(vk_get_device_proc_addr(*pDevice, "vkDestroySwapchainKHR"));
#endif
    }

    return ret;
}
}
} // namespace lx::gpu::loader