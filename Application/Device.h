#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

#include "Queue.h"
#include "SwapChain.h"

#include <vector>

class Validation;
class SwapChain;
class Managers;
class FrameBuffer;
class Command;
class SyncObjectManager;
class Scene;

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
	

private:
	void createSurface(GLFWwindow* window);
	void createPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createManagers();
	void createFrameBuffer();
	void createCommand();
	void createSyncObjectManager();
	void createScene();

	bool isDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

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

	Managers* mManagers = nullptr;

	FrameBuffer* mFrameBuffer = nullptr;

	Command* mCommand = nullptr;

	SyncObjectManager* mSyncObjectManager = nullptr;

	Scene* mScene = nullptr;

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