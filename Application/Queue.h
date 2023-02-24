#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

class Device;

class Queue
{
public:

	Queue();
	~Queue();

public:
	void acquireQueue(Device* device);

private:
	VkQueue mGraphicsQueue = VK_NULL_HANDLE;
};