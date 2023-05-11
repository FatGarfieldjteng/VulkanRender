#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"

class VertexFormat;

class TriangleVertexBuffer : public VertexBuffer
{
public:
	TriangleVertexBuffer(Device* device);
	~TriangleVertexBuffer();

public:
	virtual void init();
};