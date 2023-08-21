#include "Command.h"
#include "Queue.h"
#include "Device.h"
#include "SwapChain.h"
#include "FrameBuffer.h"
#include "Managers.h"
#include "PassManager.h"
#include "PipelineManager.h"
#include "ConstantBufferManager.h"
#include "Pipeline.h"
#include "SimpleScene.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <stdexcept>

Command::Command(VkPhysicalDevice physicalDevice,
    VkDevice logicalDevice,
    VkSurfaceKHR surface,
    unsigned int maxFramesInFligt)
    :mPhysicalDevice(physicalDevice)
    , mLogicalDevice(logicalDevice)
    , mSurface(surface)
    , mMaxFramesInFligt(maxFramesInFligt)
{
    create();
}

Command::~Command()
{
    vkDestroyCommandPool(mLogicalDevice, mCommandPool, nullptr);

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
    mCommandBuffers.resize(mMaxFramesInFligt);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = mMaxFramesInFligt;

    if (vkAllocateCommandBuffers(mLogicalDevice, 
        &allocInfo, 
        mCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Command::recordSimpleRenderingCommandBuffer(VkCommandBuffer commandBuffer,
    Scene* scene,
    SwapChain* swapChain,
    FrameBuffer* frameBuffer,
    Managers* managers,
    size_t imageIndex,
    size_t frameIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
  
    VkExtent2D extent = swapChain->getExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = managers->getPassManager()->getPass("SimplePass");
    renderPassInfo.framebuffer = frameBuffer->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearValues[2];
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = &clearValues[0];

    Pipeline* pipeline = managers->getPipelineManager()->getPipeline("SimplePipeline");

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        pipeline->getPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    int meshCount = scene->getMeshCount();

    // update WVP constant buffer
    ConstantBufferManager* constantBufferManager = managers->getConstantBufferManager();

    vkCmdBindDescriptorSets(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline->getPipelineLayout(),
        0,
        1,
        constantBufferManager->getConstantBuffer("Simple")->getDescriptorSets(frameIndex),
        0,
        nullptr);

    for (int meshIndex = 0; meshIndex < meshCount; ++meshIndex)
    {
        Mesh* mesh = scene->getMesh(meshIndex);

        VertexBuffer* vb = mesh->getVB();

        VkBuffer vertexBuffers[] = { vb->mVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        IndexBuffer* ib = mesh->getIB();
        vkCmdBindIndexBuffer(commandBuffer, ib->mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(commandBuffer, ib->mIndices, 1, 0, 0, 0);
    }

    //vkCmdDraw(commandBuffer, vb->mVertices, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Command::recordPBRRenderingCommandBuffer(VkCommandBuffer commandBuffer,
    Scene* scene,
    SwapChain* swapChain,
    FrameBuffer* frameBuffer,
    Managers* managers,
    size_t imageIndex,
    size_t frameIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkExtent2D extent = swapChain->getExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = managers->getPassManager()->getPass("SimplePass");
    renderPassInfo.framebuffer = frameBuffer->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearValues[2];
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = &clearValues[0];

    Pipeline* pipeline = managers->getPipelineManager()->getPipeline("SimplePipeline");

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline->getPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    int meshCount = scene->getMeshCount();

    // update WVP constant buffer
    ConstantBufferManager* constantBufferManager = managers->getConstantBufferManager();

    vkCmdBindDescriptorSets(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline->getPipelineLayout(),
        0,
        1,
        constantBufferManager->getConstantBuffer("Simple")->getDescriptorSets(frameIndex),
        0,
        nullptr);

    for (int meshIndex = 0; meshIndex < meshCount; ++meshIndex)
    {
        Mesh* mesh = scene->getMesh(meshIndex);

        VertexBuffer* vb = mesh->getVB();

        VkBuffer vertexBuffers[] = { vb->mVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        IndexBuffer* ib = mesh->getIB();
        vkCmdBindIndexBuffer(commandBuffer, ib->mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(commandBuffer, ib->mIndices, 1, 0, 0, 0);
    }

    //vkCmdDraw(commandBuffer, vb->mVertices, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}