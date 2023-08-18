#include "BeautyRenderPass.h"
#include "Device.h"
#include "DepthStencilBuffer.h"
#include "Scene.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

BeautyRenderPass::BeautyRenderPass(Device* device, 
    PassInfo passinfo)
    :RenderPass(device, passinfo)
{
    buildPass();
}

BeautyRenderPass::~BeautyRenderPass()
{
 
}

void BeautyRenderPass::buildPass()
{
    mDevice->createRenderPassVkRenderPass(false, false, false, false, &mRenderPass);

    mDevice->createRenderPassFrameBuffer(mRenderPass,
        mDevice->getDepthStencilBuffer()->getView(),
        mFramebuffers);

    // build pipeline

}

void BeautyRenderPass::recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex, Scene* scene)
{
	VkExtent2D extent = mDevice->getSwapChain()->getExtent();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = mRenderPass;
	renderPassInfo.framebuffer = mFramebuffers[frameIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = extent;


	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);

    int meshCount = scene->getMeshCount();

    for (int meshIndex = 0; meshIndex < meshCount; ++meshIndex)
    {
        Mesh* mesh = scene->getMesh(meshIndex);

        VertexBuffer* vb = mesh->getVB();

        VkBuffer vertexBuffers[] = { vb->mVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        IndexBuffer* ib = mesh->getIB();
        vkCmdBindIndexBuffer(commandBuffer, ib->mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        /*vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            mPipelineLayout, 0, 1, 
            &descriptorSets_[currentImage], 
            0, nullptr);*/


        vkCmdDrawIndexed(commandBuffer, ib->mIndices, 1, 0, 0, 0);
    }

	vkCmdEndRenderPass(commandBuffer);
}