#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SwapChain;
class FrameBuffer;
class Managers;

class Command
{
public:
	Command(VkPhysicalDevice physicalDevice, 
		VkDevice logicalDevice, 
		VkSurfaceKHR surface);
	~Command();

public:
	void create();
	void recordCommandBuffer(SwapChain* swapChain,
		FrameBuffer* frameBuffer,
		Managers *managers,
		size_t imageIndex);
	VkCommandBuffer get();

private:
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR mSurface = VK_NULL_HANDLE;
	VkCommandPool mCommandPool = VK_NULL_HANDLE;
	VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;
};