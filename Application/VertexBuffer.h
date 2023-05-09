#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexFormat;

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

private:
	VertexFormat* mFormat = nullptr;
	VkBuffer vertexBuffer = VK_NULL_HANDLE;
};