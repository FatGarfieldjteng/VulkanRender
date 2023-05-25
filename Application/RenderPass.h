#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device;

class RenderPass
{
public:
	RenderPass(Device* device);
	virtual ~RenderPass();

protected:
	Device* mDevice = nullptr;
};