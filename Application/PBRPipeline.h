#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Pipeline.h"

class Managers;

class PBRPipeline : public Pipeline
{
public:
	
	PBRPipeline(VkDevice logicalDevice, Managers* managers);
	virtual ~PBRPipeline();

public:
	virtual void setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos);
	virtual void setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info);
	virtual void setupPipelineLayout(VkPipelineLayoutCreateInfo& info);
	virtual VkRenderPass getRenderPass();
};