#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class SwapChain;
class FrameBuffer;
class Managers;

class Command
{
public:
	Command(VkPhysicalDevice physicalDevice, 
		VkDevice logicalDevice, 
		VkSurfaceKHR surface,
		unsigned int maxFramesInFligt = 2);
	~Command();

public:
	void create();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, 
		SwapChain* swapChain,
		FrameBuffer* frameBuffer,
		Managers *managers,
		size_t imageIndex);


	VkCommandBuffer get(size_t index)
	{
		if (index < 0 || index >= mMaxFramesInFligt)
		{
			return VK_NULL_HANDLE;
		}

		return  mCommandBuffers[index];
	}

private:
	unsigned int mMaxFramesInFligt = 2;
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR mSurface = VK_NULL_HANDLE;
	VkCommandPool mCommandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> mCommandBuffers;
};