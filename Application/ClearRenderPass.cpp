#include "ClearRenderPass.h"
#include "Device.h"
#include "DepthStencilBuffer.h"
#include "SwapChain.h"

ClearRenderPass::ClearRenderPass(Device* device)
    :RenderPass(device)
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

void ClearRenderPass::recordCommand(VkCommandBuffer commandBuffer, 
	Managers* managers, 
	size_t imageIndex,
	size_t frameIndex,
	Scene* scene)
{
	VkClearValue clearValues[2];
	clearValues[0].color = { {1.0f, 1.0f, 1.0f, 1.0f} };
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