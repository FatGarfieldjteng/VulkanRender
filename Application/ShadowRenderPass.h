#pragma once
#include "RenderPass.h"

class Device;

class ShadowRenderPass : public RenderPass
{
public:
	ShadowRenderPass(Device* device, 
		RenderPass::PassInfo passinfo);
	virtual ~ShadowRenderPass();

public:
	virtual void buildPass();
	virtual void recordCommand(VkCommandBuffer commandBuffer,
		Managers* managers,
		size_t imageIndex,
		size_t frameIndex,
		Scene* scene);

private:
	unsigned int mWidth = 2048;
	unsigned int mHeight = 2048;
};