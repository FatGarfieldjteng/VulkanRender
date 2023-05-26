#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexBuffer;
class IndexBuffer;
class Device;
class BoundingBox;

class Mesh
{
public:
	Mesh(Device* device);
	virtual ~Mesh();

public:
	virtual void init(uint32_t vertices = 0,
		VkDeviceSize vBufferSize = 0, 
		const void* vBufferData = nullptr,
		uint32_t indices = 0,
		VkDeviceSize iBufferSize = 0,
		const void* iBbufferData = nullptr,
		const BoundingBox* bbox = nullptr);

	VertexBuffer* getVB()
	{
		return mVertexBuffer;
	}

	IndexBuffer* getIB()
	{
		return mIndexBuffer;
	}

	BoundingBox* getBBox()
	{
		return mBBox;
	}

protected:
	Device* mDevice = nullptr;
	BoundingBox* mBBox = nullptr;
	VertexBuffer* mVertexBuffer = nullptr;
	IndexBuffer* mIndexBuffer = nullptr;
};