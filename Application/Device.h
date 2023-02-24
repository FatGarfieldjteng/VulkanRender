#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device
{
public:

	Device();
	~Device();

public:
	void create(VkInstance VkInstance);

private:
	bool isDeviceSuitable(VkPhysicalDevice device);

private:
	VkPhysicalDevice mDevice = VK_NULL_HANDLE;
};