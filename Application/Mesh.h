#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VertexBuffer;

class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	VertexBuffer* mVertexBuffer = nullptr;
};