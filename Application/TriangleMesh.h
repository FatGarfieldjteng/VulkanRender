#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Mesh.h"

class Device;

class TriangleMesh : public Mesh
{
public:
	TriangleMesh(Device* device);
	~TriangleMesh();

public:
	virtual void init();

};