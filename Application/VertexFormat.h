#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class VertexFormat
{
public:
	VertexFormat();
	~VertexFormat();

	virtual void init() = 0;

	VkVertexInputBindingDescription mBinding;
	std::vector<VkVertexInputAttributeDescription> mAttribute;
};