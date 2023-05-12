#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Mesh.h"

class Device;

class TriangleMesh : public Mesh
{
public:
	TriangleMesh(Device* device);
	virtual ~TriangleMesh();

public:
	virtual void init(uint32_t vertices = 0,
		VkDeviceSize vBufferSize = 0,
		void* vBufferData = nullptr,
		uint32_t indices = 0,
		VkDeviceSize iBufferSize = 0,
		void* iBbufferData = nullptr);

};