#pragma once
#include <map>
#include <string>
#include <vector>

class RenderTask;
class RenderTaskResource;
class RenderTaskLinkInfo;

class RenderTaskGraph
{
public:
	RenderTaskGraph();
	virtual ~RenderTaskGraph();

public:
	virtual void createTasks() = 0;
	virtual void linkTask(RenderTaskLinkInfo* linkInfos);
	
	RenderTask* addTask(const std::string& ID, RenderTask* task);
	RenderTask* getTask(const std::string& ID);

	virtual void compile();
	virtual void execute();

protected:
	std::map<std::string, RenderTask*> mIDToTask;
	std::vector<RenderTaskLinkInfo*> mLinkInfos;
	std::vector<RenderTask*> mCompiledTask;
};