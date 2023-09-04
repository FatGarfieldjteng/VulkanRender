#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "RenderPass.h"

class Device;
class Scene;

class ClearRenderPass: public RenderPass
{
public:
	ClearRenderPass(Device* device);
	virtual ~ClearRenderPass();

public:
	
	virtual void buildPass();
	virtual void recordCommand(VkCommandBuffer commandBuffer, 
		Managers* managers, 
		size_t imageIndex,
		size_t frameIndex,
		Scene* scene = nullptr);

};