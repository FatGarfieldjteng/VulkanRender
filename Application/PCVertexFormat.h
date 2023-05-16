#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "VertexFormat.h"
#include <vector>

class PCVertexFormat : public VertexFormat
{
public:
	PCVertexFormat();
	~PCVertexFormat();

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
	};

	virtual void init();
};