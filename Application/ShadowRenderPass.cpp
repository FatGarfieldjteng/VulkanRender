#include "ShadowRenderPass.h"
#include "Device.h"
#include "DepthStencilBuffer.h"
#include "Scene.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Managers.h"
#include "PipelineManager.h"
#include "ConstantBufferManager.h"
#include "TransitResourceManager.h"
#include "TransitResource.h"
#include "PassManager.h"
#include "Pipeline.h"
#include "PBRMaterial.h"
#include "SimpleScene.h"
#include "ConstantBuffer.h"
#include "FrameBuffer.h"

ShadowRenderPass::ShadowRenderPass(Device* device)
    :RenderPass(device)
{

}

ShadowRenderPass::~ShadowRenderPass()
{
 
}

void ShadowRenderPass::buildPass()
{
    // create framebuffers
    unsigned int maxFramesInFligt = mDevice->getMaxFramesInFlight();

    mFramebuffers.resize(maxFramesInFligt);

    Managers* managers = mDevice->getManagers();

    TransitResourceManager* transitResourceManager = mDevice->getManagers()->getTransitResourceManager();

    PassManager* passManager = managers->getPassManager();

    TransitResource* resource = transitResourceManager->getResource("depth");

    for (size_t i = 0; i < maxFramesInFligt; ++i)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.flags = 0;
        framebufferInfo.renderPass = passManager->getPass("ShadowPass");;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &resource->mImageViews[i];
        framebufferInfo.width = mWidth;
        framebufferInfo.height = mHeight;
        framebufferInfo.layers = 1;

        vkCreateFramebuffer(mDevice->getLogicalDevice(), &framebufferInfo, nullptr, &mFramebuffers[i]);
    }
}

void ShadowRenderPass::recordCommand(VkCommandBuffer commandBuffer,
    Managers* managers,
    size_t imageIndex,
    size_t frameIndex,
    Scene* scene)
{
    TransitResourceManager* transitResourceManager = mDevice->getManagers()->getTransitResourceManager();
    TransitResource* resource = transitResourceManager->getResource("depth");
    
    int width = resource->mWidth;
    int height = resource->mHeight;

    PassManager* passManager = managers->getPassManager();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = passManager->getPass("ShadowPass");
    
    renderPassInfo.framebuffer = mFramebuffers[frameIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent.width = (uint32_t)width;
    renderPassInfo.renderArea.extent.height = (uint32_t)height;
    
    VkClearValue clearValue;
    // color clear value is not relevant
    clearValue.depthStencil = { 1.0f, 0 };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    PipelineManager* pipelineManager = managers->getPipelineManager();
    Pipeline* pipeline = pipelineManager->getPipeline("Shadow");

    vkCmdSetDepthBias(
        commandBuffer,
        depthBiasConstant,
        0.0f,
        depthBiasSlope);

    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline->getPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent.width = (uint32_t)width;
    scissor.extent.height = (uint32_t)height;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


    // update constant buffer, not finished!!
    ConstantBufferManager* constantBufferManager = managers->getConstantBufferManager();
    ConstantBuffer* shadowConstantBuffer = constantBufferManager->getConstantBuffer("Shadow");

    int meshCount = scene->getMeshCount();
    int materialCount = scene->getMaterialCount();

    SimpleScene* simplescene = dynamic_cast<SimpleScene*>(scene);

    for (int meshIndex = 0; meshIndex < meshCount; ++meshIndex)
    {
        Mesh* mesh = scene->getMesh(meshIndex);

        VertexBuffer* vb = mesh->getVB();

        VkBuffer vertexBuffers[] = { vb->mVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        IndexBuffer* ib = mesh->getIB();
        vkCmdBindIndexBuffer(commandBuffer, ib->mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline->getPipelineLayout(), 0, 1,
            shadowConstantBuffer->getDescriptorSets(frameIndex),
            0, nullptr);

        vkCmdDrawIndexed(commandBuffer, ib->mIndices, 1, 0, 0, 0);
    }

    vkCmdEndRenderPass(commandBuffer);
}