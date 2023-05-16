#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Device;

class DepthStencilBuffer
{
public:
	DepthStencilBuffer(Device *device);
	virtual ~DepthStencilBuffer();

public:
	void init(VkExtent2D extent);

	VkFormat getFormat();

	VkImageView getView();

private:
	Device* mDevice = nullptr;
	VkImage mImage = VK_NULL_HANDLE;
	VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
	VkImageView mImageView = VK_NULL_HANDLE;

	VkFormat mFormat;
};