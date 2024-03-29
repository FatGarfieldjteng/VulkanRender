#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <string>

class Device;

class CubeTexture
{
public:
	CubeTexture(Device *device);
	virtual ~CubeTexture();

public:
	void load(const std::string& file);

public:
	Device* mDevice = nullptr;

	int mOriginalWidth = 0;
	int mOriginalHeight = 0;
	int mChannels = 0;

	VkImage mImage = VK_NULL_HANDLE;
	VkImageView mImageView;
	VkSampler mSampler;

	VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
};