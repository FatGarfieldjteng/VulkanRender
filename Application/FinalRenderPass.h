#pragma once
#include "RenderPass.h"

class Device;

class FinalRenderPass : public RenderPass
{
public:
	FinalRenderPass(Device* device, 
		RenderPass::PassInfo passinfo);
	virtual ~FinalRenderPass();

public:
	virtual void buildPass();
	virtual void recordCommand(VkCommandBuffer commandBuffer, 
		Managers* managers, 
		size_t imageIndex,
		size_t frameIndex,
		Scene* scene = nullptr);
};