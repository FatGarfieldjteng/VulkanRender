#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <string>

class Device;

class Texture
{
public:
	Texture(Device *device);
	virtual ~Texture();

public:
	void load(const std::string& file);

public:
	Device* mDevice = nullptr;

	int mWidth = 0;
	int mHeight = 0;
	int mChannels = 0;

	VkImage mImage = VK_NULL_HANDLE;
	VkImageView mImageView;
	VkSampler mSampler;

	VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
};