#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Mesh.h"

class TriangleMesh : public Mesh
{
public:
	TriangleMesh();
	~TriangleMesh();

public:
	virtual void init();

};