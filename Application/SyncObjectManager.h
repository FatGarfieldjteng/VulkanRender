#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>

class SyncObjectManager
{
public:

    SyncObjectManager(VkDevice logicalDevice);

    ~SyncObjectManager();

public:
    void createSyncObjects();

    void addSemaphore(const std::string& ID, VkSemaphore semaphore);
    void addFence(const std::string& ID, VkFence fence);

    VkSemaphore getSemaphore(const std::string& ID);
    VkFence getFence(const std::string& ID);
  
private:
    
    VkDevice mLogicalDevice = VK_NULL_HANDLE;
    std::map<std::string, VkSemaphore> mIDToSemaphore;
    std::map<std::string, VkFence > mIDToFence;
};
