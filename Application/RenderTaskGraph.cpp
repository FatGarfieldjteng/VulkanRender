#include "RenderTaskGraph.h"
#include "RenderTask.h"

RenderTaskGraph::RenderTaskGraph()
{
    
}

RenderTaskGraph::~RenderTaskGraph()
{
 
}

void RenderTaskGraph::linkPass(RenderTask* srcPass, RenderTask* dstPass, RenderTaskResource* resource)
{
	srcPass->addOutputResource(resource);
	dstPass->addInputResource(resource);
}

RenderTask* RenderTaskGraph::getTask(const std::string& ID)
{
	std::map<std::string, RenderTask*>::iterator it = mIDToTask.find(ID);

	if (it != mIDToTask.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}

void RenderTaskGraph::compile()
{

}

void RenderTaskGraph::execute()
{

}