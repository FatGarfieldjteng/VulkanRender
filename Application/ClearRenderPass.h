#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "RenderPass.h"
#include <vector>

class Device;
class Scene;

class ClearRenderPass: public RenderPass
{
public:
	ClearRenderPass(Device* device,
		RenderPass::PassInfo passinfo);
	virtual ~ClearRenderPass();

public:
	
	virtual void buildPass();
	virtual void recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex);

private:
	std::vector<VkFramebuffer> mFramebuffers;
		
};