#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Command
{
public:
	Command();
	~Command();

public:
	void create();

private:
	VkCommandPool mCommandPool = VK_NULL_HANDLE;
	VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;
};