#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

class Device
{
public:

	Device();
	~Device();

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool isComplete() 
		{
			return graphicsFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

public:
	void create(VkInstance VkInstance);
	void acquireQueue(VkQueue* graphicsQueue);

private:

	void createPhysicalDevice(VkInstance VkInstance);
	void createLogicalDevice();

	bool isDeviceSuitable(VkPhysicalDevice device);

private:
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	QueueFamilyIndices mQueueFamilyIndices;
};