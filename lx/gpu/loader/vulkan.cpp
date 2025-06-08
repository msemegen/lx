/*
 *   Name: vulkan.cpp
 *   Copyright (c) Mateusz Semegen and contributors. All rights reserved.
 */

// this
#include <lx/common/out.hpp>
#include <lx/utils/logger.hpp>
#define VMA_IMPLEMENTATION
#include <lx/gpu/loader/vulkan.hpp>

// std
#include <cassert>

namespace {
HMODULE vk_driver_handle = nullptr;
} // namespace

PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;

PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = nullptr;

// instance level functions
PFN_vkDestroyInstance vkDestroyInstance = nullptr;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
#if defined(VK_VERSION_1_1)
PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2 = nullptr;
#endif
#if defined(VK_KHR_surface)
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
#endif
#if defined(VK_KHR_win32_surface)
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
#endif
#if defined(VK_EXT_debug_utils)
PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = nullptr;
PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;
#endif

// device level functions
#if defined(VK_VERSION_1_0)
PFN_vkDestroyDevice vkDestroyDevice = nullptr;
PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
PFN_vkCreateShaderModule vkCreateShaderModule = nullptr;
PFN_vkDestroyShaderModule vkDestroyShaderModule = nullptr;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = nullptr;
PFN_vkDestroyPipeline vkDestroyPipeline = nullptr;
PFN_vkAllocateMemory vkAllocateMemory = nullptr;
PFN_vkFreeMemory vkFreeMemory = nullptr;
PFN_vkMapMemory vkMapMemory = nullptr;
PFN_vkUnmapMemory vkUnmapMemory = nullptr;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges = nullptr;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = nullptr;
PFN_vkBindBufferMemory vkBindBufferMemory = nullptr;
PFN_vkBindImageMemory vkBindImageMemory = nullptr;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
PFN_vkCreateBuffer vkCreateBuffer = nullptr;
PFN_vkDestroyBuffer vkDestroyBuffer = nullptr;
PFN_vkCreateImage vkCreateImage = nullptr;
PFN_vkDestroyImage vkDestroyImage = nullptr;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer = nullptr;
PFN_vkCreateImageView vkCreateImageView = nullptr;
PFN_vkDestroyImageView vkDestroyImageView = nullptr;
PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
PFN_vkResetCommandPool vkResetCommandPool = nullptr;
PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
PFN_vkEndCommandBuffer vkEndCommandBuffer;
PFN_vkQueueSubmit vkQueueSubmit;
#endif
#if defined(VK_KHR_swapchain)
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
#endif

namespace {
template<typename Function> Function load_instance_level_function(VkInstance vk_instance_a, const char* name_a)
{
    Function pfn = reinterpret_cast<Function>(vkGetInstanceProcAddr(vk_instance_a, name_a));

    if (nullptr == pfn)
    {
        log_err("Cannot load {} Vulkan API function", name_a);
    }
    else
    {
        log_inf("{} successfuly loaded", name_a);
    }

    return pfn;
}

template<typename Function> Function load_device_level_function(VkDevice vk_device_a, const char* name_a)
{
    Function pfn = reinterpret_cast<Function>(vkGetDeviceProcAddr(vk_device_a, name_a));

    if (nullptr == pfn)
    {
        log_err("Cannot load {} Vulkan API function", name_a);
    }
    else
    {
        log_inf("{} successfuly loaded", name_a);
    }

    return pfn;
}

template<typename Function> void unload_function(lx::common::out<Function> function)
{
    if (nullptr != *function)
    {
        *function = nullptr;
    }
}
} // namespace

#define LOAD_INSTANCE_LEVEL_FUNCTION(function) function = load_instance_level_function<decltype(function)>(*pInstance, #function)
#define LOAD_DEVICE_LEVEL_FUNCTION(function) function = load_device_level_function<decltype(function)>(*pDevice, #function)

namespace lx::gpu::loader {
bool vulkan::load()
{
    vk_driver_handle = LoadLibrary("vulkan-1.dll");
    if (nullptr != vk_driver_handle)
    {
        version = VK_API_VERSION_1_0;

        vkGetInstanceProcAddr =
            reinterpret_cast<decltype(vkGetInstanceProcAddr)>(GetProcAddress(vk_driver_handle, "vkGetInstanceProcAddr"));
        vkGetDeviceProcAddr = reinterpret_cast<decltype(vkGetDeviceProcAddr)>(GetProcAddress(vk_driver_handle, "vkGetDeviceProcAddr"));

        vkEnumerateInstanceExtensionProperties = reinterpret_cast<decltype(vkEnumerateInstanceExtensionProperties)>(
            GetProcAddress(vk_driver_handle, "vkEnumerateInstanceExtensionProperties"));
        vkEnumerateInstanceLayerProperties = reinterpret_cast<decltype(vkEnumerateInstanceLayerProperties)>(
            GetProcAddress(vk_driver_handle, "vkEnumerateInstanceLayerProperties"));
        vkEnumerateInstanceVersion =
            reinterpret_cast<decltype(vkEnumerateInstanceVersion)>(GetProcAddress(vk_driver_handle, "vkEnumerateInstanceVersion"));

        if (nullptr != vkEnumerateInstanceVersion)
        {
            std::uint32_t vk_version;
            vkEnumerateInstanceVersion(&vk_version);

            unload_function(lx::common::out(vkEnumerateInstanceVersion));

            version = vk_version;
        }

        const lx::common::Version::Components vr = version;
        log_inf("Vulkan version installed on system: {}.{}.{}", vr.major, vr.minor, vr.patch);

        return true;
    }
    else
    {
        log_omg("No Vulkan installed!");
    }

    return false;
}
void vulkan::release()
{
    assert(nullptr != vk_driver_handle);

    unload_function(lx::common::out(vkGetInstanceProcAddr));
    unload_function(lx::common::out(vkGetDeviceProcAddr));

    unload_function(lx::common::out(vkEnumerateInstanceExtensionProperties));
    unload_function(lx::common::out(vkEnumerateInstanceLayerProperties));

    // instance level functions
    unload_function(lx::common::out(vkDestroyInstance));
    unload_function(lx::common::out(vkGetPhysicalDeviceProperties));
    unload_function(lx::common::out(vkGetPhysicalDeviceFeatures));
    unload_function(lx::common::out(vkEnumeratePhysicalDevices));
    unload_function(lx::common::out(vkEnumerateDeviceExtensionProperties));
    unload_function(lx::common::out(vkGetPhysicalDeviceQueueFamilyProperties));
    unload_function(lx::common::out(vkGetPhysicalDeviceMemoryProperties));
#if defined(VK_KHR_surface)
    unload_function(lx::common::out(vkGetPhysicalDeviceSurfaceCapabilitiesKHR));
    unload_function(lx::common::out(vkGetPhysicalDeviceSurfaceFormatsKHR));
    unload_function(lx::common::out(vkGetPhysicalDeviceSurfacePresentModesKHR));
    unload_function(lx::common::out(vkGetPhysicalDeviceSurfaceSupportKHR));
    unload_function(lx::common::out(vkDestroySurfaceKHR));
#endif
#if defined(VK_KHR_win32_surface)
    unload_function(lx::common::out(vkCreateWin32SurfaceKHR));
    unload_function(lx::common::out(vkGetPhysicalDeviceWin32PresentationSupportKHR));
#endif
#if defined(VK_EXT_debug_utils)
    unload_function(lx::common::out(vkCreateDebugUtilsMessengerEXT));
    unload_function(lx::common::out(vkDestroyDebugUtilsMessengerEXT));
#endif

// device level functions
#if defined(VK_VERSION_1_0)
    unload_function(lx::common::out(vkDestroyDevice));
    unload_function(lx::common::out(vkGetDeviceQueue));
    unload_function(lx::common::out(vkCreateShaderModule));
    unload_function(lx::common::out(vkDestroyShaderModule));
    unload_function(lx::common::out(vkCreateGraphicsPipelines));
    unload_function(lx::common::out(vkDestroyPipeline));
    unload_function(lx::common::out(vkAllocateMemory));
    unload_function(lx::common::out(vkFreeMemory));
    unload_function(lx::common::out(vkMapMemory));
    unload_function(lx::common::out(vkUnmapMemory));
    unload_function(lx::common::out(vkFlushMappedMemoryRanges));
    unload_function(lx::common::out(vkInvalidateMappedMemoryRanges));
    unload_function(lx::common::out(vkBindBufferMemory));
    unload_function(lx::common::out(vkBindImageMemory));
    unload_function(lx::common::out(vkGetBufferMemoryRequirements));
    unload_function(lx::common::out(vkGetImageMemoryRequirements));
    unload_function(lx::common::out(vkCreateBuffer));
    unload_function(lx::common::out(vkDestroyBuffer));
    unload_function(lx::common::out(vkCreateImage));
    unload_function(lx::common::out(vkDestroyImage));
    unload_function(lx::common::out(vkCmdCopyBuffer));
    unload_function(lx::common::out(vkCreateImageView));
    unload_function(lx::common::out(vkDestroyImageView));
    unload_function(lx::common::out(vkCreateCommandPool));
    unload_function(lx::common::out(vkResetCommandPool));
    unload_function(lx::common::out(vkDestroyCommandPool));
    unload_function(lx::common::out(vkAllocateCommandBuffers));
    unload_function(lx::common::out(vkFreeCommandBuffers));
    unload_function(lx::common::out(vkBeginCommandBuffer));
    unload_function(lx::common::out(vkEndCommandBuffer));
    unload_function(lx::common::out(vkQueueSubmit));
#endif
#if defined(VK_KHR_swapchain)
    unload_function(lx::common::out(vkCreateSwapchainKHR));
    unload_function(lx::common::out(vkGetSwapchainImagesKHR));
    unload_function(lx::common::out(vkDestroySwapchainKHR));
#endif

    FreeLibrary(vk_driver_handle);

    vk_driver_handle = nullptr;
}

extern "C" {
VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                                                const VkAllocationCallbacks* pAllocator,
                                                VkInstance* pInstance)
{
    assert(nullptr != vk_driver_handle);

    PFN_vkCreateInstance vk_create_instance =
        reinterpret_cast<decltype(vk_create_instance)>(GetProcAddress(vk_driver_handle, "vkCreateInstance"));
    VkResult ret = vk_create_instance(pCreateInfo, pAllocator, pInstance);

    if (VK_SUCCESS == ret)
    {
        LOAD_INSTANCE_LEVEL_FUNCTION(vkDestroyInstance);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceProperties);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceFeatures);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkEnumeratePhysicalDevices);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkEnumerateDeviceExtensionProperties);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
#if defined(VK_KHR_surface)
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkDestroySurfaceKHR);
#endif
#if defined(VK_KHR_win32_surface)
        LOAD_INSTANCE_LEVEL_FUNCTION(vkCreateWin32SurfaceKHR);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkGetPhysicalDeviceWin32PresentationSupportKHR);
#endif
#if defined(VK_EXT_debug_utils)
        LOAD_INSTANCE_LEVEL_FUNCTION(vkCreateDebugUtilsMessengerEXT);
        LOAD_INSTANCE_LEVEL_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
#endif
    }

    return ret;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkInstance instance,
                                              VkPhysicalDevice physicalDevice,
                                              const VkDeviceCreateInfo* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDevice* pDevice)
{
    PFN_vkCreateDevice vk_create_device = reinterpret_cast<decltype(vk_create_device)>(vkGetInstanceProcAddr(instance, "vkCreateDevice"));
    VkResult ret = vk_create_device(physicalDevice, pCreateInfo, pAllocator, pDevice);

    if (VK_SUCCESS == ret)
    {
#if defined(VK_VERSION_1_0)
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyDevice);
        LOAD_DEVICE_LEVEL_FUNCTION(vkGetDeviceQueue);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateShaderModule);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyShaderModule);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateGraphicsPipelines);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyPipeline);
        LOAD_DEVICE_LEVEL_FUNCTION(vkAllocateMemory);
        LOAD_DEVICE_LEVEL_FUNCTION(vkFreeMemory);
        LOAD_DEVICE_LEVEL_FUNCTION(vkMapMemory);
        LOAD_DEVICE_LEVEL_FUNCTION(vkUnmapMemory);
        LOAD_DEVICE_LEVEL_FUNCTION(vkFlushMappedMemoryRanges);
        LOAD_DEVICE_LEVEL_FUNCTION(vkInvalidateMappedMemoryRanges);
        LOAD_DEVICE_LEVEL_FUNCTION(vkBindBufferMemory);
        LOAD_DEVICE_LEVEL_FUNCTION(vkBindImageMemory);
        LOAD_DEVICE_LEVEL_FUNCTION(vkGetBufferMemoryRequirements);
        LOAD_DEVICE_LEVEL_FUNCTION(vkGetImageMemoryRequirements);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateBuffer);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyBuffer);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateImage);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyImage);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCmdCopyBuffer);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateImageView);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyImageView);
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateCommandPool);
        LOAD_DEVICE_LEVEL_FUNCTION(vkResetCommandPool);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroyCommandPool);
        LOAD_DEVICE_LEVEL_FUNCTION(vkAllocateCommandBuffers);
        LOAD_DEVICE_LEVEL_FUNCTION(vkFreeCommandBuffers);
        LOAD_DEVICE_LEVEL_FUNCTION(vkBeginCommandBuffer);
        LOAD_DEVICE_LEVEL_FUNCTION(vkEndCommandBuffer);
        LOAD_DEVICE_LEVEL_FUNCTION(vkQueueSubmit);
#endif
#if defined(VK_KHR_swapchain)
        LOAD_DEVICE_LEVEL_FUNCTION(vkCreateSwapchainKHR);
        LOAD_DEVICE_LEVEL_FUNCTION(vkGetSwapchainImagesKHR);
        LOAD_DEVICE_LEVEL_FUNCTION(vkDestroySwapchainKHR);
#endif
    }
    return ret;
}
}
} // namespace lx::gpu::loader