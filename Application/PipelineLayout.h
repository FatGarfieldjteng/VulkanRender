#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class ShaderManager;

class PipelineLayout
{
public:
	
	PipelineLayout();
	~PipelineLayout();

public:
	void create(VkDevice logicalDevice, ShaderManager *shaderManager);

private:
	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
	VkRenderPass menderPass = VK_NULL_HANDLE;
};