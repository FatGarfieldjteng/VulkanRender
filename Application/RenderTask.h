#pragma once

#include <vector>

class RenderTaskResource;

class RenderTask
{
public:
	struct PassInfo
	{
		bool isFirstPass = false;
		bool isLastPass = false;
	};
public:
	RenderTask();
	virtual ~RenderTask();

public:
	void addInputResource(RenderTaskResource* resource);
	void addOutputResource(RenderTaskResource* resource);

protected:
	PassInfo mPassInfo;

	std::vector<RenderTaskResource*> mInputs;
	std::vector<RenderTaskResource*> mOutputs;
};