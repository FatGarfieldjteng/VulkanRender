#include "ClearRenderPass.h"
#include "Device.h"
#include "DepthStencilBuffer.h"
#include "SwapChain.h"

ClearRenderPass::ClearRenderPass(Device* device, 
    RenderPass::PassInfo passinfo)
    :RenderPass(device, passinfo)
{
    buildPass();
}

ClearRenderPass::~ClearRenderPass()
{
 
}

void ClearRenderPass::buildPass()
{
    mDevice->createRenderPassVkRenderPass(true, false, true, true, &mRenderPass);

    mDevice->createRenderPassFrameBuffer(mRenderPass, 
        mDevice->getDepthStencilBuffer()->getView(), 
        mFramebuffers);
}

void ClearRenderPass::recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex)
{
	VkClearValue clearValues[2];
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkExtent2D extent = mDevice->getSwapChain()->getExtent();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = mRenderPass;
	renderPassInfo.framebuffer = mFramebuffers[frameIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = extent;
	renderPassInfo.clearValueCount = static_cast<uint32_t>(mDevice->getDepthStencilBuffer() != nullptr ? 2 : 1),
	renderPassInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(commandBuffer);
}