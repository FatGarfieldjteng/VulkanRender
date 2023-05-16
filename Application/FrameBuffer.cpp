#include "FrameBuffer.h"
#include "SwapChain.h"
#include "DepthStencilBuffer.h"
#include "Managers.h"
#include "PassManager.h"
#include <stdexcept>

FrameBuffer::FrameBuffer(VkDevice logicalDevice, 
    SwapChain* swapChain,
    DepthStencilBuffer* depthStencilBuffer,
    Managers* managers)
    :mLogicalDevice(logicalDevice)
    , mSwapChain(swapChain)
    , mDepthStencilBuffer(depthStencilBuffer)
    , mManagers(managers)
{
    createFramebuffers();
}

FrameBuffer::~FrameBuffer()
{
    for (auto framebuffer : mFramebuffers)
    {
        vkDestroyFramebuffer(mLogicalDevice, framebuffer, nullptr);
    }
}

void FrameBuffer::createFramebuffers()
{
    size_t frameBufferCount = mSwapChain->getViewsCount();
    mFramebuffers.resize(frameBufferCount);

    VkExtent2D extent = mSwapChain->getExtent();

    for (size_t i = 0; i < frameBufferCount; ++i)
    {
        VkImageView attachments[] =
        {
            mSwapChain->getView(i),
            mDepthStencilBuffer->getView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = mManagers->getPassManager()->getPass("SimplePass");
        framebufferInfo.attachmentCount = 2;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(mLogicalDevice,
            &framebufferInfo, 
            nullptr, 
            &mFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

VkFramebuffer FrameBuffer::getFrameBuffer(size_t index)
{
    return mFramebuffers[index];
}