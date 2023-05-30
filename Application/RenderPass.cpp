#include "RenderPass.h"
#include "Device.h"

RenderPass::RenderPass(Device* device, 
    PassInfo passinfo)
    :mDevice(device)
    , mPassInfo(passinfo)
{
    
}

RenderPass::~RenderPass()
{
    for (auto framebuffer : mFramebuffers)
    {
        vkDestroyFramebuffer(mDevice->getLogicalDevice(), framebuffer, nullptr);
    }

    vkDestroyRenderPass(mDevice->getLogicalDevice(), mRenderPass, nullptr);
}