#pragma once

#include <vector>

class RenderTaskResource;

class RenderTask
{
public:
	RenderTask();
	virtual ~RenderTask();

public:
	void addInputResource(RenderTaskResource* resource);
	void addOutputResource(RenderTaskResource* resource);

	virtual void execute() = 0;

protected:
	std::vector<RenderTaskResource*> mInputs;
	std::vector<RenderTaskResource*> mOutputs;
};