#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexBuffer;
class IndexBuffer;
class Device;

class Mesh
{
public:
	Mesh(Device* device);
	virtual ~Mesh();

public:
	virtual void init(uint32_t vertices = 0, 
		VkDeviceSize vBufferSize = 0, 
		void* vBufferData = nullptr,
		uint32_t indices = 0,
		VkDeviceSize iBufferSize = 0,
		void* iBbufferData = nullptr);

	VertexBuffer* getVB()
	{
		return mVertexBuffer;
	}

	IndexBuffer* getIB()
	{
		return mIndexBuffer;
	}

protected:
	Device* mDevice = nullptr;
	VertexBuffer* mVertexBuffer = nullptr;
	IndexBuffer* mIndexBuffer = nullptr;
};