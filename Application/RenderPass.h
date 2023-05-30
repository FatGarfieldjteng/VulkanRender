#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device;
class Scene;
class FrameBuffer;

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
	virtual void recordCommand() = 0;

protected:
	Device* mDevice = nullptr;
	PassInfo mPassInfo;

	// Vulkan objects
	VkRenderPass mRenderPass = nullptr;
	VkPipelineLayout mPipelineLayout = nullptr;
	VkPipeline mipeline = nullptr;

	FrameBuffer* mFrameBuffer = nullptr;
};