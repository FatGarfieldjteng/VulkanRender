#include "FinalRenderPass.h"
#include "Device.h"
#include "DepthStencilBuffer.h"

FinalRenderPass::FinalRenderPass(Device* device, 
    RenderPass::PassInfo passinfo)
    :RenderPass(device, passinfo)
{
    buildPass();
}

FinalRenderPass::~FinalRenderPass()
{
 
}

void FinalRenderPass::buildPass()
{
    mDevice->createRenderPassVkRenderPass(false, true, false, false, &mRenderPass);

    mDevice->createRenderPassFrameBuffer(mRenderPass,
        mDevice->getDepthStencilBuffer()->getView(),
        mFramebuffers);
}

void FinalRenderPass::recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex)
{
	VkExtent2D extent = mDevice->getSwapChain()->getExtent();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = mRenderPass;
	renderPassInfo.framebuffer = mFramebuffers[frameIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = extent;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(commandBuffer);
}