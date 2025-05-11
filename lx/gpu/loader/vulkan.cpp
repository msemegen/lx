/*
 *   Name: vulkan.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#define VMA_IMPLEMENTATION
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cassert>

namespace {
HMODULE vk_handle = nullptr;
} // namespace

PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;

PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

// instance level functions
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
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
PFN_vkAllocateMemory vkAllocateMemory;
PFN_vkFreeMemory vkFreeMemory;
PFN_vkMapMemory vkMapMemory;
PFN_vkUnmapMemory vkUnmapMemory;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
PFN_vkBindBufferMemory vkBindBufferMemory;
PFN_vkBindImageMemory vkBindImageMemory;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
PFN_vkCreateBuffer vkCreateBuffer;
PFN_vkDestroyBuffer vkDestroyBuffer;
PFN_vkCreateImage vkCreateImage;
PFN_vkDestroyImage vkDestroyImage;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
PFN_vkCreateImageView vkCreateImageView;
PFN_vkDestroyImageView vkDestroyImageView;
#endif
#if defined(VK_VERSION_1_1)
PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
PFN_vkBindBufferMemory2 vkBindBufferMemory2;
PFN_vkBindImageMemory2 vkBindImageMemory2;
PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
#endif
#if defined(VK_VERSION_1_3)
PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
#endif
#if defined(VK_KHR_swapchain)
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
#endif

#define LOAD_INSTANCE_LAYER_FUNCTION(function) function = reinterpret_cast<decltype(function)>(vkGetInstanceProcAddr(*pInstance, #function))
#define LOAD_DEVICE_LAYER_FUNCTION(function) function = reinterpret_cast<decltype(function)>(vkGetDeviceProcAddr(*pDevice, #function))

namespace lx::gpu::loader {
bool vulkan::load()
{
    vk_handle = LoadLibrary("vulkan-1.dll");
    if (nullptr != vk_handle)
    {
        vkGetInstanceProcAddr = reinterpret_cast<decltype(vkGetInstanceProcAddr)>(GetProcAddress(vk_handle, "vkGetInstanceProcAddr"));
        vkGetDeviceProcAddr = reinterpret_cast<decltype(vkGetDeviceProcAddr)>(GetProcAddress(vk_handle, "vkGetDeviceProcAddr"));

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
        LOAD_INSTANCE_LAYER_FUNCTION(vkDestroyInstance);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceProperties);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceFeatures);
        LOAD_INSTANCE_LAYER_FUNCTION(vkEnumeratePhysicalDevices);
        LOAD_INSTANCE_LAYER_FUNCTION(vkEnumerateDeviceExtensionProperties);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
#if defined(VK_KHR_surface)
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
        LOAD_INSTANCE_LAYER_FUNCTION(vkDestroySurfaceKHR);
#endif
#if defined(VK_KHR_win32_surface)
        LOAD_INSTANCE_LAYER_FUNCTION(vkCreateWin32SurfaceKHR);
        LOAD_INSTANCE_LAYER_FUNCTION(vkGetPhysicalDeviceWin32PresentationSupportKHR);
#endif
#if defined(VK_EXT_debug_utils)
        LOAD_INSTANCE_LAYER_FUNCTION(vkCreateDebugUtilsMessengerEXT);
        LOAD_INSTANCE_LAYER_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
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
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroyDevice);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetDeviceQueue);
        LOAD_DEVICE_LAYER_FUNCTION(vkCreateShaderModule);
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroyShaderModule);
        LOAD_DEVICE_LAYER_FUNCTION(vkCreateGraphicsPipelines);
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroyPipeline);
        LOAD_DEVICE_LAYER_FUNCTION(vkAllocateMemory);
        LOAD_DEVICE_LAYER_FUNCTION(vkFreeMemory);
        LOAD_DEVICE_LAYER_FUNCTION(vkMapMemory);
        LOAD_DEVICE_LAYER_FUNCTION(vkUnmapMemory);
        LOAD_DEVICE_LAYER_FUNCTION(vkFlushMappedMemoryRanges);
        LOAD_DEVICE_LAYER_FUNCTION(vkInvalidateMappedMemoryRanges);
        LOAD_DEVICE_LAYER_FUNCTION(vkBindBufferMemory);
        LOAD_DEVICE_LAYER_FUNCTION(vkBindImageMemory);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetBufferMemoryRequirements);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetImageMemoryRequirements);
        LOAD_DEVICE_LAYER_FUNCTION(vkCreateBuffer);
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroyBuffer);
        LOAD_DEVICE_LAYER_FUNCTION(vkCreateImage);
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroyImage);
        LOAD_DEVICE_LAYER_FUNCTION(vkCmdCopyBuffer);
        LOAD_DEVICE_LAYER_FUNCTION(vkCreateImageView);
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroyImageView);
#endif
#if defined(VK_VERSION_1_1)
        LOAD_DEVICE_LAYER_FUNCTION(vkGetBufferMemoryRequirements2);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetImageMemoryRequirements2);
        LOAD_DEVICE_LAYER_FUNCTION(vkBindBufferMemory2);
        LOAD_DEVICE_LAYER_FUNCTION(vkBindImageMemory2);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetPhysicalDeviceMemoryProperties2);
#endif
#if defined(VK_VERSION_1_3)
        LOAD_DEVICE_LAYER_FUNCTION(vkGetDeviceBufferMemoryRequirements);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetDeviceImageMemoryRequirements);
#endif
#if defined(VK_KHR_swapchain)
        LOAD_DEVICE_LAYER_FUNCTION(vkCreateSwapchainKHR);
        LOAD_DEVICE_LAYER_FUNCTION(vkGetSwapchainImagesKHR);
        LOAD_DEVICE_LAYER_FUNCTION(vkDestroySwapchainKHR);
#endif
    }

    return ret;
}
}
} // namespace lx::gpu::loader