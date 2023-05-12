#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device;

class IndexBuffer
{
public:
	IndexBuffer(Device* device);
	virtual ~IndexBuffer();

public:
	virtual void init(uint32_t indices = 0, VkDeviceSize bufferSize = 0, void* bufferData = nullptr);

	Device* mDevice = nullptr;
	VkBuffer mIndexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory mIndexBufferMemory = VK_NULL_HANDLE;

	uint32_t mIndices = 0;
};