#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Device;
class Scene;

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
	virtual void recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex) = 0;

protected:
	Device* mDevice = nullptr;
	PassInfo mPassInfo;

	// Vulkan objects
	VkRenderPass mRenderPass = VK_NULL_HANDLE;
	VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
	VkPipeline mPipeline = VK_NULL_HANDLE;

protected:
	std::vector<VkFramebuffer> mFramebuffers;
};