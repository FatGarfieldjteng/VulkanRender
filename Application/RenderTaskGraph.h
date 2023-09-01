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
	virtual void linkPass(RenderTaskLinkInfo* linkInfos);
	void compile();
	void execute();
	RenderTask* getTask(const std::string& ID);
protected:
	std::map<std::string, RenderTask*> mIDToTask;
	std::vector<RenderTaskLinkInfo*> mLinkInfos;
	std::vector<RenderTask*> mCompiledTask;
};