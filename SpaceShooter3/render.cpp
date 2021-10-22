#include "render.h"

#include "third_party/stb_ds.h"
#include "GLFW/glfw3.h"

#include <memory>
#include <cassert>
#include <iostream>

//undesirable
#include <vector>
#include <set>

void* GetNextAlignedPtr(const void* ptr, const uint8_t alignment) 
{	
	void* result = (void*)(((uintptr_t)ptr + (alignment-1)) & ~(uintptr_t)0x0F);
	bool withinScratch = (char*)result < (char*)Scratch + SCRATCH_SIZE;
	assert(withinScratch);
	return result;
}

void InitVulkan()
{
	AllocScratch();

	CreateInstance();
}

void CreateInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport()) {
		assert(false);
		//throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Space";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
	uint32_t extensionCount = 0;
	const char** extensions = (const char**)Scratch;
	if (enableValidationLayers) {		

		extensions[extensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
		extensionCount++;
	}
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (size_t glfwExt = 0; glfwExt < glfwExtensionCount; glfwExt++)
	{
		extensions[extensionCount] = glfwExtensions[glfwExt];
		extensionCount++;
	}

	createInfo.enabledExtensionCount = extensionCount;
	createInfo.ppEnabledExtensionNames = extensions;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = validationLayerCount;
		createInfo.ppEnabledLayerNames = validationLayers;
		
		debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = debugCallback;

		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	
	if (vkCreateInstance(&createInfo, nullptr, &Vulkan.Instance) != VK_SUCCESS) {
		//throw std::runtime_error("failed to create instance!");
		assert(false);
	}
	
	//------------------------------------------

	if (enableValidationLayers) {
		if (CreateDebugUtilsMessengerEXT(Vulkan.Instance, &debugCreateInfo, nullptr, &Vulkan.DebugMessenger) != VK_SUCCESS) {
			assert(false);
			//throw std::runtime_error("failed to set up debug messenger!");
		}
	}

	//------------------------- Physical Device

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Vulkan.Instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		assert(false);
		//throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);	
	vkEnumeratePhysicalDevices(Vulkan.Instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device, Vulkan.Surface))
		{
			Vulkan.PhysicalDevice = device;
			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
			VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
			if (counts & VK_SAMPLE_COUNT_64_BIT) { Settings.msaaSamples = VK_SAMPLE_COUNT_64_BIT; }
			else if (counts & VK_SAMPLE_COUNT_32_BIT) { Settings.msaaSamples = VK_SAMPLE_COUNT_32_BIT; }
			else if (counts & VK_SAMPLE_COUNT_16_BIT) { Settings.msaaSamples = VK_SAMPLE_COUNT_16_BIT; }
			else if (counts & VK_SAMPLE_COUNT_8_BIT) { Settings.msaaSamples = VK_SAMPLE_COUNT_8_BIT; }
			else if (counts & VK_SAMPLE_COUNT_4_BIT) { Settings.msaaSamples = VK_SAMPLE_COUNT_4_BIT; }
			else if (counts & VK_SAMPLE_COUNT_2_BIT) { Settings.msaaSamples = VK_SAMPLE_COUNT_2_BIT; }
			else { Settings.msaaSamples = VK_SAMPLE_COUNT_1_BIT; }
			break;
		}
	}


	if (Vulkan.PhysicalDevice == VK_NULL_HANDLE) {
		assert(false);
		//throw std::runtime_error("failed to find a suitable GPU!");
	}

	
}

void createLogicalDevice(VkSurfaceKHR surface, bool validation, const std::vector<const char*> validationLayers)
{
	uint32_t graphicsFamIdx = -1;
	uint32_t presentFamIdx = -1;
	findQueueFamilies(Vulkan.PhysicalDevice, Vulkan.Surface, graphicsFamIdx, presentFamIdx);

	//QueueFamilyIndices indices = physDevice.findQueueFamilies(physDevice.device, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { graphicsFamIdx, presentFamIdx };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = deviceExtensionsCount;
	createInfo.ppEnabledExtensionNames = deviceExtensions;

	if (validation) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	//todo cleanup
	if (vkCreateDevice(Vulkan.PhysicalDevice, &createInfo, nullptr, &Vulkan.Device) != VK_SUCCESS) {
		assert(false);
		//throw std::runtime_error("failed to create logical device!");
	}
	//device = physDevice.device.createDevice(vk::DeviceCreateInfo(createInfo));

	vkGetDeviceQueue(Vulkan.Device, graphicsFamIdx, 0, &Vulkan.graphicsQueue);
	vkGetDeviceQueue(Vulkan.Device, presentFamIdx, 0, &Vulkan.presentQueue);
	//graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0);
	//presentQueue = device.getQueue(indices.presentFamily.value(), 0);
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<const char*> requiredExtensions{ deviceExtensions[0] };

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface, uint32_t& outGraphicsFamilyIdx,
	uint32_t& outPresentFamilyIdx)
{
	uint32_t graphicsFamilyIdx = -1;
	uint32_t presentFamilyIdx = -1;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamilyIdx = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			presentFamilyIdx = i;
		}

		if (presentFamilyIdx != -1 && graphicsFamilyIdx != -1) {
			break;
		}

		i++;
	}

	outGraphicsFamilyIdx = graphicsFamilyIdx;
	outPresentFamilyIdx = presentFamilyIdx;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
	
	uint32_t graphicsFamilyIdx = -1;
	uint32_t presentFamilyIdx = -1;
	findQueueFamilies(device, surface, graphicsFamilyIdx, presentFamilyIdx);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return presentFamilyIdx != -1 && graphicsFamilyIdx != -1 && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool CheckValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	VkLayerProperties* AvailableLayers = (VkLayerProperties*)Scratch;
	vkEnumerateInstanceLayerProperties(&layerCount, AvailableLayers);

	const char* layerName = validationLayers[0];
		
	bool layerFound = false;

	for (size_t i = 0; i < layerCount; i++)
	{
		if (strcmp(layerName, AvailableLayers[i].layerName) == 0) {
			layerFound = true;
			break;
		}
	}
	
	return layerFound;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

void createSurface(GLFWwindow* window)
{
	if (glfwCreateWindowSurface(Vulkan.Instance, window, nullptr, &Vulkan.Surface) != VK_SUCCESS) {
		assert(false);
		//throw std::runtime_error("failed to create window surface!");
	}
}
