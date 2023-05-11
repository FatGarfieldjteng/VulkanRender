#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexFormat;
class Device;

class VertexBuffer
{
public:
	VertexBuffer(Device* device);
	~VertexBuffer();

public:
	virtual void init() = 0;

	Device* mDevice = nullptr;
	VkBuffer mVertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory mVertexBufferMemory = VK_NULL_HANDLE;

	uint32_t mSize = 0;
};