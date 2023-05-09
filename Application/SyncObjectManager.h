#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <vector>

class SyncObjectManager
{
public:

    SyncObjectManager(VkDevice logicalDevice,
        unsigned int maxFramesInFligt = 2);

    ~SyncObjectManager();

public:
    void createSyncObjects();

    VkSemaphore getBackBufferReadySemaphore(size_t index)
    {
        if (index < 0 || index >= mMaxFramesInFligt)
        {
            return VK_NULL_HANDLE;
        }

        return  mBackBufferReadySemaphore[index];
    }

    VkSemaphore getPresentReadySemaphore(size_t index)
    {
        if (index < 0 || index >= mMaxFramesInFligt)
        {
            return VK_NULL_HANDLE;
        }

        return  mPresentReadySemaphore[index];
    }

    VkFence geFrameFence(size_t index)
    {
        if (index < 0 || index >= mMaxFramesInFligt)
        {
            return VK_NULL_HANDLE;
        }

        return  mFrameFence[index];
    }

private:
    void createFrameLevelSyncObjects();
  
private:
    
    unsigned int mMaxFramesInFligt = 2;
    VkDevice mLogicalDevice = VK_NULL_HANDLE;
    std::vector<VkSemaphore> mBackBufferReadySemaphore;
    std::vector<VkSemaphore> mPresentReadySemaphore;
    std::vector<VkFence> mFrameFence;
};
