#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Device;
class Scene;
class PassManager;
class Managers;

class RenderPass
{
public:
	struct PassInfo
	{
		bool isFirstPass = false;
		bool isLastPass = false;
	};
public:
	RenderPass(Device* device,
		PassInfo passinfo);
	virtual ~RenderPass();

public:
	
	virtual void buildPass() = 0;
	virtual void recordCommand(VkCommandBuffer commandBuffer, 
		Managers* managers, 
		size_t frameIndex, 
		Scene* scene = nullptr) = 0;

protected:
	Device* mDevice = nullptr;
	PassInfo mPassInfo;

	// Vulkan objects
	VkRenderPass mRenderPass = VK_NULL_HANDLE;

protected:
	std::vector<VkFramebuffer> mFramebuffers;
};