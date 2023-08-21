#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Pipeline.h"

class Managers;

class SimplePipeline : public Pipeline
{
public:
	
	SimplePipeline(VkDevice logicalDevice, 
		Scene* scene, 
		Managers* managers);
	virtual ~SimplePipeline();

protected:
	virtual void setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos);
	virtual void setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info);
	virtual void setupPipelineLayout(VkPipelineLayoutCreateInfo& info);
	virtual VkRenderPass getRenderPass();
};