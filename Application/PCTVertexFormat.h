#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "VertexFormat.h"
#include <vector>

class PCTVertexFormat : public VertexFormat
{
public:
	PCTVertexFormat();
	~PCTVertexFormat();

	struct Vertex {

		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		bool operator==(const Vertex& other) const 
		{
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	};

	virtual void init();
};

namespace std {
	template<> struct hash<PCTVertexFormat::Vertex> {
		size_t operator()(PCTVertexFormat::Vertex const& vertex) const \
		{
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}