#pragma once
#include "RenderPass.h"

class Device;

class ShadowRenderPass : public RenderPass
{
public:
	ShadowRenderPass(Device* device);
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

	// depth bias for shadow map generation
	float depthBiasConstant = 0.05f;

	float depthBiasSlope = 0.75f;
};