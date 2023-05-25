#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "VertexFormat.h"
#include <vector>

class PBRVertexFormat : public VertexFormat
{
public:
	PBRVertexFormat();
	~PBRVertexFormat();

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;

	};

	virtual void init();
};