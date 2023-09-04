#include "VulkanRenderTaskGraph.h"
#include "RenderTask.h"
#include "RenderTaskLinkInfo.h"
#include "ShadowRenderPass.h"
#include "BeautyRenderPass.h"
#include "Device.h"
#include "RenderPassManager.h"
#include "TransitResourceManager.h"

VulkanRenderTaskGraph::VulkanRenderTaskGraph()
{
    
}

VulkanRenderTaskGraph::~VulkanRenderTaskGraph()
{
 
}

void VulkanRenderTaskGraph::createTasks()
{
    RenderTask* shadowTask = createShadowTask();
    RenderTask* beautyTask = createBeautyTask();

    enum class RenderTaskLinkInfo::LinkType type = 
        RenderTaskLinkInfo::LinkType::LINK_TYPE_RENDERTARGET_DEPTH;

    enum class RenderTaskLinkInfo::LinkFormat format = 
        RenderTaskLinkInfo::LinkFormat::LINK_FORMAT_D32;

    RenderTaskLinkInfo* info = new RenderTaskLinkInfo(shadowTask, 
        beautyTask,
        type,
        format);
    
    linkTask(info);
}

void VulkanRenderTaskGraph::compile()
{
    mCompiledTask.push_back(getTask("shadow"));
    mCompiledTask.push_back(getTask("beauty"));

    // init transit resources
    std::vector<RenderTaskLinkInfo*>::iterator it;
    for (it = mLinkInfos.begin(); it != mLinkInfos.end(); ++it)
    {
        switch ((*it)->mLinkType)
        {
        case RenderTaskLinkInfo::LinkType::LINK_TYPE_RENDERTARGET_DEPTH:
            {
                mDevice->getTransitResourceManager()->createTransitDepthMap();
            }
            break;
        default:
            break;
        }
    }

    for (size_t i = 0; i < mCompiledTask.size(); ++i)
    {
        
    }

    
}

void VulkanRenderTaskGraph::execute()
{
    
}

void VulkanRenderTaskGraph::setDevice(Device* device)
{
    mDevice = device;
}

RenderTask* VulkanRenderTaskGraph::createShadowTask()
{
    ShadowRenderPass* pass = new ShadowRenderPass(mDevice);
    return addTask("shadow", pass);
}

RenderTask* VulkanRenderTaskGraph::createBeautyTask()
{
    BeautyRenderPass* pass = new BeautyRenderPass(mDevice);
    return addTask("beauty", pass);
}