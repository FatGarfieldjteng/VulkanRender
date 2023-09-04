#include "RenderTaskGraph.h"
#include "RenderTask.h"

RenderTaskGraph::RenderTaskGraph()
{
    
}

RenderTaskGraph::~RenderTaskGraph()
{
	for (size_t linkInfoIndex = 0; linkInfoIndex < mLinkInfos.size(); ++linkInfoIndex)
	{
		delete mLinkInfos[linkInfoIndex];
	}

	std::map<std::string, RenderTask*>::iterator it;

	for (it = mIDToTask.begin(); it != mIDToTask.end(); ++it)
	{
		delete  it->second;
	}
}

void RenderTaskGraph::linkTask(RenderTaskLinkInfo* linkInfos)
{
	mLinkInfos.push_back(linkInfos);
}

RenderTask* RenderTaskGraph::addTask(const std::string& ID, RenderTask* task)
{
	RenderTask* taskFound = getTask(ID);

	if (taskFound)
	{
		return taskFound;
	}

	mIDToTask[ID] = task;

	return task;
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
	for (size_t i = 0; i < mCompiledTask.size(); ++i)
	{
		mCompiledTask[i]->execute();
	}
}