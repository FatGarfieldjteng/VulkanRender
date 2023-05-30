#include "ClearRenderPass.h"
#include "Device.h"

ClearRenderPass::ClearRenderPass(Device* device, 
    RenderPass::PassInfo passinfo)
    :RenderPass(device, passinfo)
{
    
}

ClearRenderPass::~ClearRenderPass()
{
 
}

void ClearRenderPass::buildPass()
{
    mDevice->createRenderPassVkRenderPass(true, false, true, true, &mRenderPass);
}

void ClearRenderPass::recordCommand()
{

}