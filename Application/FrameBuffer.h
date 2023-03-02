#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class SwapChain;
class Managers;

class FrameBuffer
{
public:
	FrameBuffer(VkDevice logicalDevice, 
		SwapChain* swapChain,
		Managers* managers);
	~FrameBuffer();

public:
	VkFramebuffer getFrameBuffer(size_t index);

private:

	void createFramebuffers();


private:
	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	SwapChain* mSwapChain = nullptr;
	Managers* mManagers;
	std::vector<VkFramebuffer> mFramebuffers;
};