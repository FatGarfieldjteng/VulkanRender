#include "BeautyRenderPass.h"
#include "Device.h"
#include "DepthStencilBuffer.h"
#include "Scene.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Managers.h"
#include "PipelineManager.h"
#include "ConstantBufferManager.h"
#include "PassManager.h"
#include "Pipeline.h"
#include "PBRPipeline.h"
#include "ConstantBuffer.h"
#include "FrameBuffer.h"

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

}

void BeautyRenderPass::recordCommand(VkCommandBuffer commandBuffer,
    Managers* managers,
    size_t imageIndex,
    size_t frameIndex,
    Scene* scene)
{
	VkExtent2D extent = mDevice->getSwapChain()->getExtent();
    PassManager* passManager = managers->getPassManager();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = passManager->getPass("SimplePass");
    
	renderPassInfo.framebuffer = mDevice->getFrameBuffer()->getFrameBuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = extent;
    VkClearValue clearValues[2];
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = &clearValues[0];

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    PipelineManager* pipelineManager = managers->getPipelineManager();
    Pipeline* pipeline = pipelineManager->getPipeline("PBR");
    PBRPipeline* pbrpipeline = dynamic_cast<PBRPipeline*>(pipeline);

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

    ConstantBufferManager *constantBufferManager = managers->getConstantBufferManager();
    ConstantBuffer* PBRConstantBuffer = constantBufferManager->getConstantBuffer("PBR");

    int meshCount = scene->getMeshCount();
    int materialCount = scene->getMaterialCount();

    for (int meshIndex = 0; meshIndex < meshCount; ++meshIndex)
    {
        Mesh* mesh = scene->getMesh(meshIndex);

        VertexBuffer* vb = mesh->getVB();

        VkBuffer vertexBuffers[] = { vb->mVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        IndexBuffer* ib = mesh->getIB();
        vkCmdBindIndexBuffer(commandBuffer, ib->mIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        int descriptorSetIndex = frameIndex * materialCount + mesh->getMaterialIndex();
 
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
            pipeline->getPipelineLayout(), 0, 1,
            PBRConstantBuffer->getDescriptorSets(descriptorSetIndex),
            0, nullptr);

        // push color constant
        vkCmdPushConstants(
            commandBuffer,
            pipeline->getPipelineLayout(),
            VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(PBRPipeline::MaterialValue),
            &pbrpipeline->getMaterialValues()[meshIndex]);

        vkCmdDrawIndexed(commandBuffer, ib->mIndices, 1, 0, 0, 0);
    }

	vkCmdEndRenderPass(commandBuffer);
}