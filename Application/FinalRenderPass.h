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
	virtual void recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex);
};