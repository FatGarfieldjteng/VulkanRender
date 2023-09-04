#pragma once
#include "RenderTaskGraph.h"

class Device;
class RenderTask;

class VulkanRenderTaskGraph : public RenderTaskGraph
{
public:
	VulkanRenderTaskGraph();
	virtual ~VulkanRenderTaskGraph();

public:
	void setDevice(Device* device);

protected:
	RenderTask* createShadowTask();
	RenderTask* createBeautyTask();

public:
	virtual void createTasks();

	virtual void compile();
	virtual void execute();

private:
	Device* mDevice = nullptr;
};