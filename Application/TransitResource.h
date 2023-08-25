#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Device;

class TransitResource
{
public:
	TransitResource(Device *device,
		unsigned int maxFramesInFligt = 2);
	virtual ~TransitResource();

protected:
	Device* mDevice = nullptr;

	unsigned int mMaxFramesInFligt = 2;

	int mWidth = 0;
	int mHeight = 0;
	int mChannels = 0;
	
};