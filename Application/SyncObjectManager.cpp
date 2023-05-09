#include "SyncObjectManager.h"
#include <stdexcept>

SyncObjectManager::SyncObjectManager(VkDevice logicalDevice,
	unsigned int maxFramesInFligt)
	:mLogicalDevice(logicalDevice)
	, mMaxFramesInFligt(maxFramesInFligt)
{
	createSyncObjects();
}

SyncObjectManager::~SyncObjectManager()
{
	for (unsigned int inFlightIndex = 0; inFlightIndex < mMaxFramesInFligt; ++inFlightIndex)
	{
		vkDestroySemaphore(mLogicalDevice, mBackBufferReadySemaphore[inFlightIndex], nullptr);

		vkDestroySemaphore(mLogicalDevice, mPresentReadySemaphore[inFlightIndex], nullptr);

		vkDestroyFence(mLogicalDevice, mFrameFence[inFlightIndex], nullptr);
	}
}

void SyncObjectManager::createSyncObjects()
{
	createFrameLevelSyncObjects();
}

void SyncObjectManager::createFrameLevelSyncObjects()
{
	mBackBufferReadySemaphore.resize(mMaxFramesInFligt);
	mPresentReadySemaphore.resize(mMaxFramesInFligt);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	mFrameFence.resize(mMaxFramesInFligt);

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


	for (unsigned int inFlightIndex = 0; inFlightIndex < mMaxFramesInFligt; ++inFlightIndex)
	{
		if (vkCreateSemaphore(mLogicalDevice,
			&semaphoreInfo, nullptr,
			&mBackBufferReadySemaphore[inFlightIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create backbuffer available Semaphore!");
		}

		if (vkCreateSemaphore(mLogicalDevice,
			&semaphoreInfo, nullptr,
			&mPresentReadySemaphore[inFlightIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create backbuffer available Semaphore!");
		}

		if (vkCreateFence(mLogicalDevice,
			&fenceInfo,
			nullptr,
			&mFrameFence[inFlightIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create in flight Fence!");
		}

	}
}