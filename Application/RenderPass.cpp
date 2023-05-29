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
 
}