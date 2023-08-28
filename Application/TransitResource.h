#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class Device;

class TransitResource
{
public:
	TransitResource(Device *device,
		unsigned int maxFramesInFligt = 2);
	virtual ~TransitResource();

protected:
	virtual void create() = 0;

protected:
	Device* mDevice = nullptr;

	unsigned int mMaxFramesInFligt = 2;

public:
	int mWidth = 0;
	int mHeight = 0;
	int mChannels = 0;

	std::vector<VkImage> mImages;
	std::vector<VkImageView> mImageViews;
	std::vector<VkSampler> mSamplers;

	std::vector<VkDeviceMemory> mDeviceMemories;
};