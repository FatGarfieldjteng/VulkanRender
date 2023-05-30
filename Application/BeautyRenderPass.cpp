#include "BeautyRenderPass.h"
#include "Device.h"

BeautyRenderPass::BeautyRenderPass(Device* device, 
    PassInfo passinfo)
    :RenderPass(device, passinfo)
{
    
}

BeautyRenderPass::~BeautyRenderPass()
{
 
}

void BeautyRenderPass::buildPass()
{

}

void BeautyRenderPass::recordCommand(VkCommandBuffer commandBuffer, size_t frameIndex)
{

}