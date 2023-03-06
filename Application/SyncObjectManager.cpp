#include "SyncObjectManager.h"
#include <stdexcept>

SyncObjectManager::SyncObjectManager(VkDevice logicalDevice)
	:mLogicalDevice(logicalDevice)
{
	createSyncObjects();
}

SyncObjectManager::~SyncObjectManager()
{
	std::map<std::string, VkSemaphore>::iterator itSemaphore;

	for (itSemaphore = mIDToSemaphore.begin(); itSemaphore != mIDToSemaphore.end(); ++itSemaphore)
	{
		vkDestroySemaphore(mLogicalDevice, itSemaphore->second, nullptr);
	}


	std::map<std::string, VkFence>::iterator itFence;

	for (itFence = mIDToFence.begin(); itFence != mIDToFence.end(); ++itFence)
	{
		vkDestroyFence(mLogicalDevice, itFence->second, nullptr);
	}
}

void SyncObjectManager::createSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore semaphore = VK_NULL_HANDLE;

	if (vkCreateSemaphore(mLogicalDevice,
		&semaphoreInfo, nullptr,
		&semaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create backbuffer available Semaphore!");
	}

	addSemaphore("BackBufferAvailable", semaphore);
		
	if (vkCreateSemaphore(mLogicalDevice,
		&semaphoreInfo, nullptr,
		&semaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render finish Semaphore!");
	}

	addSemaphore("RenderFinish", semaphore);
	
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkFence inFlight = VK_NULL_HANDLE;
	if (vkCreateFence(mLogicalDevice, 
		&fenceInfo, 
		nullptr, 
		&inFlight) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create in flight Fence!");
	}

	addFence("InFlight", inFlight);
}


void SyncObjectManager::addSemaphore(const std::string& ID, VkSemaphore semaphore)
{
	mIDToSemaphore[ID] = semaphore;
}

void SyncObjectManager::addFence(const std::string& ID, VkFence fence)
{
	mIDToFence[ID] = fence;
}

VkSemaphore SyncObjectManager::getSemaphore(const std::string& ID)
{
	std::map<std::string, VkSemaphore>::iterator it = mIDToSemaphore.find(ID);

	if (it != mIDToSemaphore.end())
	{
		return  it->second;
	}
	else
	{
		return VK_NULL_HANDLE;
	}
}

VkFence SyncObjectManager::getFence(const std::string& ID)
{
	std::map<std::string, VkFence >::iterator it = mIDToFence.find(ID);

	if (it != mIDToFence.end())
	{
		return  it->second;
	}
	else
	{
		return VK_NULL_HANDLE;
	}
}