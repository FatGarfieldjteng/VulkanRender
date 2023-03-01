#include "Command.h"
#include "Queue.h"
#include <stdexcept>

Command::Command()
{
 
}

Command::~Command()
{

}

void Command::create()
{
    // create CommandPool
    Queue::QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    // create CommandBuffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocInfo, &mCommandBuffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}