#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexBuffer;
class Device;

class Mesh
{
public:
	Mesh(Device* device);
	virtual ~Mesh();

public:
	virtual void init() = 0;

	VertexBuffer* getVB()
	{
		return mVertexBuffer;
	}

protected:
	Device* mDevice = nullptr;
	VertexBuffer* mVertexBuffer = nullptr;
};