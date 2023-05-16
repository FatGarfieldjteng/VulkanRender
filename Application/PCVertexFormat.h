#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
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

		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color;
		}
	};

	virtual void init();
};

namespace std {
	template<> struct hash<PCVertexFormat::Vertex> {
		size_t operator()(PCVertexFormat::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1);
		}
	};
}