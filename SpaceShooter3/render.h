#pragma once

#include "vulkan/vulkan.h"

//#include <optional>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

constexpr int32_t validationLayerCount = 1;
const char* validationLayers[validationLayerCount] = {
	"VK_LAYER_KHRONOS_validation"
	//,"VK_LAYER_LUNARG_api_dump"
};

constexpr int32_t deviceExtensionsCount = 1;
const char* deviceExtensions[deviceExtensionsCount] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}



struct {
	VkInstance Instance;
	VkDebugUtilsMessengerEXT DebugMessenger;
	VkSurfaceKHR Surface;
	VkPhysicalDevice PhysicalDevice;
	VkDevice Device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;
} Vulkan;

struct {
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
} Settings;

constexpr size_t SCRATCH_SIZE = 1024;
void* Scratch = nullptr;

void AllocScratch() {
	Scratch = malloc(SCRATCH_SIZE);
}

void* GetNextAlignedPtr(const void* ptr, const uint8_t alignment);

void InitVulkan();

void CreateInstance();

bool CheckValidationLayerSupport();

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

void createSurface(GLFWwindow* window);
