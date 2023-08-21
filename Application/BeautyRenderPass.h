#pragma once
#include "RenderPass.h"

class Device;


class BeautyRenderPass : public RenderPass
{
public:
	BeautyRenderPass(Device* device, 
		PassInfo passinfo);
	virtual ~BeautyRenderPass();

public:
	virtual void buildPass();
	virtual void recordCommand(VkCommandBuffer commandBuffer, 
		Managers* managers,
		size_t frameIndex, 
		Scene* scene = nullptr);

private:
	VkPipeline mPipeline = VK_NULL_HANDLE;
	VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
};