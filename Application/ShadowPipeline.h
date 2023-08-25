#pragma once

#define GLFW_INCLUDE_VULKAN
#include "Pipeline.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

class Managers;

class ShadowPipeline : public Pipeline
{
public:
	
	ShadowPipeline(VkDevice logicalDevice,
		Scene* scene, 
		Managers* managers);
	virtual ~ShadowPipeline();

public:
	virtual void setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos);
	virtual void setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info);

	// because shadow pass does not need color buffer, thus this function need to be reimplemented
	virtual void setupColorBlendState(VkPipelineColorBlendStateCreateInfo& info,
		const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState);
	// need to add depthBias state, , thus this function need to be reimplemented
	virtual void setupDynamicState(VkPipelineDynamicStateCreateInfo& info,
		std::vector<VkDynamicState>& dynamicStates);
	virtual void setupPipelineLayout(VkPipelineLayoutCreateInfo& info);
	virtual VkRenderPass getRenderPass();
};