#pragma once
#include "RenderTaskGraph.h"

class VulkanRenderTaskGraph : public RenderTaskGraph
{
public:
	VulkanRenderTaskGraph();
	virtual ~VulkanRenderTaskGraph();

protected:
	void createShadowTask();
	void createBeautyTask();

public:
	virtual void createTasks();
	virtual void linkPass(RenderTask* srcPass, RenderTask* dstPass, RenderTaskResource* resource);
};