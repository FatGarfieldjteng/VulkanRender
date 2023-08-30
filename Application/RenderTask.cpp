#include "RenderTask.h"

RenderTask::RenderTask()
{
    
}

RenderTask::~RenderTask()
{
 
}

void RenderTask::addInputResource(RenderTaskResource* resource)
{
    mInputs.push_back(resource);
}

void RenderTask::addOutputResource(RenderTaskResource* resource)
{
    mOutputs.push_back(resource);
}