#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Validation
{
public:

	Validation();
	~Validation();

public:
	void setVkInstance();

	bool checkValidationLayerSupport();

	size_t getValidationLayersSize() const;

	const char* const* Validation::getValidationLayersData() const;

	void setupVkDebugUtilsMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void createDebugMessenger(VkInstance VkInstance);

	void destroyDebugMessenger(VkInstance VkInstance);

private:

	const std::vector<const char*> mValidationLayers = 
	{
		"VK_LAYER_KHRONOS_validation"
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		const VkAllocationCallbacks* pAllocator);

private:
	VkInstance mVkInstance = nullptr;
	VkDebugUtilsMessengerEXT mDebugMessenger = nullptr;
};