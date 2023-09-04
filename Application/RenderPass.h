#pragma once

#define GLFW_INCLUDE_VULKAN

#include "RenderTask.h"

#include <GLFW/glfw3.h>
#include <vector>

class Device;
class Scene;
class PassManager;
class Managers;

class RenderPass : public RenderTask
{
public:
	RenderPass(Device* device);
	virtual ~RenderPass();

public:
	
	virtual void buildPass() = 0;
	virtual void recordCommand(VkCommandBuffer commandBuffer, 
		Managers* managers, 
		size_t imageIndex,
		size_t frameIndex,
		Scene* scene = nullptr) = 0;

protected:
	Device* mDevice = nullptr;

	// Vulkan objects
	VkRenderPass mRenderPass = VK_NULL_HANDLE;

protected:
	std::vector<VkFramebuffer> mFramebuffers;
};