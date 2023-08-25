#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class Managers;
class Scene;

class Pipeline
{
public:
	
	Pipeline(VkDevice logicalDevice, 
		Scene* scene,
		Managers* managers);
	virtual ~Pipeline();

public:
	void create();
	VkPipeline getPipeline();
	VkPipelineLayout getPipelineLayout();

protected:
	// components set up needed for Pipeline creation
	virtual void setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos) = 0;
	virtual void setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info) = 0;
	virtual void setupInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info);
	virtual void setupViewportState(VkPipelineViewportStateCreateInfo& info);
	virtual void setupRasterizationState(VkPipelineRasterizationStateCreateInfo& info);
	virtual void setupMultisampleState(VkPipelineMultisampleStateCreateInfo& info);
	virtual void setupDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info);
	virtual void setupColorBlendState(VkPipelineColorBlendStateCreateInfo& info, 
		const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState);
	virtual void setupDynamicState(VkPipelineDynamicStateCreateInfo& info, 
		std::vector<VkDynamicState>& dynamicStates);
	virtual void setupPipelineLayout(VkPipelineLayoutCreateInfo& info) = 0;
	virtual VkRenderPass getRenderPass() = 0;
protected:

	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	Scene* mScene = nullptr;
	Managers* mManagers = nullptr;
	VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
	VkPipeline mPipeline = VK_NULL_HANDLE;
};