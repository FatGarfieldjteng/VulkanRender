#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Managers;

class Pipeline
{
public:
	
	Pipeline();
	~Pipeline();

public:
	void create(VkDevice logicalDevice, Managers *managers);

private:

	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline;
};