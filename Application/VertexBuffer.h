#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexFormat;
class Device;

class VertexBuffer
{
public:
	VertexBuffer(Device* device);
	virtual ~VertexBuffer();

public:
	virtual void init(uint32_t vertices = 0, VkDeviceSize bufferSize = 0, void* bufferData = nullptr);

	Device* mDevice = nullptr;
	VkBuffer mVertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory mVertexBufferMemory = VK_NULL_HANDLE;

	uint32_t mVertices = 0;
};