#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "Queue.h"
#include "SwapChain.h"

#include <vector>

class Validation;
class SwapChain;
class DepthStencilBuffer;
class Managers;
class FrameBuffer;
class Command;
class SyncObjectManager;
class Scene;
class Camera;

class Device
{
public:

	Device(bool enableValidationLayers, 
		Validation* validation,
		unsigned int maxFramesInFligt = 2);
	~Device();
public:
	void create(VkInstance VkInstance, GLFWwindow* window);
	void acquireQueue(Queue::Type type, VkQueue* queue);
	void drawFrame();
	void waitIdle();
	VkFormat findDepthFormat();

	VkPhysicalDevice getPhysicalDevice()
	{
		return mPhysicalDevice;
	}

	VkDevice getLogicalDevice()
	{
		return mLogicalDevice;
	}

	void createBuffer(VkDeviceSize size, 
		VkBufferUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkBuffer& buffer, 
		VkDeviceMemory& bufferMemory);

	void copyBuffer(VkBuffer srcBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize size);

	void createImage(uint32_t width,
		uint32_t height,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory);

	VkImageView createImageView(VkImage image, 
		VkFormat format, 
		VkImageAspectFlags aspectFlags);

	VkSampler createSampler();

	void transitImageLayout(VkImage image, 
		VkFormat format, 
		VkImageLayout oldLayout, 
		VkImageLayout newLayout);

	void copyBufferToImage(VkBuffer buffer, 
		VkImage image, 
		uint32_t width, 
		uint32_t height);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	void createSurface(GLFWwindow* window);
	void createPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createDepthStencilBuffer();
	void createManagers();
	void createFrameBuffer();
	void createCommand();
	void createSyncObjectManager();
	void createScene();
	void createCamera();

	bool isDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	void beginCopyCommand();
	void endCopyCommand();

private:
	unsigned int mMaxFramesInFligt = 2;

	const bool mEnableValidationLayers = false;
	Validation* mValidation = nullptr;

	Queue::QueueFamilyIndices mQueueFamilyIndices;
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkDevice mLogicalDevice = VK_NULL_HANDLE;
	GLFWwindow* mWindow = nullptr;
	VkSurfaceKHR mSurface = VK_NULL_HANDLE;
	VkInstance mVkInstance = VK_NULL_HANDLE;

	Queue* mGraphicsQueue = nullptr;
	Queue* mPresentQueue = nullptr;

	SwapChain* mSwapChain = nullptr;

	DepthStencilBuffer* mDepthStencilBuffer = nullptr;

	Managers* mManagers = nullptr;

	FrameBuffer* mFrameBuffer = nullptr;

	Command* mCommand = nullptr;

	SyncObjectManager* mSyncObjectManager = nullptr;

	Scene* mScene = nullptr;

	Camera* mCamera = nullptr;

	uint32_t mFrameIndex = 0;

	// copy CommandPool and CopyCommand
	VkCommandPool mCopyCommandPool = VK_NULL_HANDLE;
	VkCommandBuffer mCopyCommandBuffer = VK_NULL_HANDLE;

	// swap chain extension
	const std::vector<const char*> mSwapChainExtensionName = 
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
};