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

private:
	bool isDeviceSuitable(VkPhysicalDevice device);

private:
	VkPhysicalDevice mDevice = VK_NULL_HANDLE;
};