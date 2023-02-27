#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class SwapChain
{
public:
	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapChain();
	~SwapChain();

public:
	void createSwapChain(VkPhysicalDevice physicalDevice, 
		VkDevice logicalDevice, 
		GLFWwindow* window, 
		VkSurfaceKHR surface);
	
	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

private:

	// select optimal surface format, present mode and swap extent
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

	void createImageViews();

private:
	VkDevice mLogicalDevice = VK_NULL_HANDLE;;
	VkFormat mSwapChainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D mSwapChainExtent = {
			static_cast<uint32_t>(0),
			static_cast<uint32_t>(0)
	};
	VkSwapchainKHR mSwapChain = VK_NULL_HANDLE;

	// image is created as the swapchain creation process, thus no need to free them
	std::vector<VkImage> mImages;
	std::vector<VkImageView> mImageViews;
};