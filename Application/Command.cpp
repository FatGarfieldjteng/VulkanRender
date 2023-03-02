#include "Command.h"
#include "Queue.h"
#include "Device.h"
#include "SwapChain.h"
#include "FrameBuffer.h"
#include "Managers.h"
#include "PassManager.h"
#include "PipelineManager.h"
#include "Pipeline.h"
#include <stdexcept>

Command::Command(VkPhysicalDevice physicalDevice,
    VkDevice logicalDevice,
    VkSurfaceKHR surface)
    :mPhysicalDevice(physicalDevice)
    , mLogicalDevice(logicalDevice)
    , mSurface(surface)
{
    create();
}

Command::~Command()
{

}

void Command::create()
{
    // create CommandPool
    Queue::QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(mPhysicalDevice, mSurface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(mLogicalDevice, 
        &poolInfo, 
        nullptr, 
        &mCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    // create CommandBuffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(mLogicalDevice, 
        &allocInfo, 
        &mCommandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Command::recordCommandBuffer(SwapChain* swapChain,
    FrameBuffer* frameBuffer,
    Managers* managers,
    size_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
  
    VkExtent2D extent = swapChain->getExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = managers->getPassManager()->getPass("SimplePass");
    renderPassInfo.framebuffer = frameBuffer->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(mCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(mCommandBuffer, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        managers->getPipelineManager()->getPipeline("SimplePipeline")->get());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);

    vkCmdDraw(mCommandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(mCommandBuffer);

    if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}