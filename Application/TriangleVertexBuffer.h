#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"

class VertexFormat;

class TriangleVertexBuffer : public VertexBuffer
{
public:
	TriangleVertexBuffer(Device* device);
	virtual ~TriangleVertexBuffer();

public:
	virtual void init(uint32_t vertices = 0, VkDeviceSize bufferSize = 0, void* bufferData = nullptr);
};