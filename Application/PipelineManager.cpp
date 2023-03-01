#include "PipelineManager.h"
#include "Pipeline.h"

PipelineManager::PipelineManager(VkDevice logicalDevice, Managers* managers)
	:mLogicalDevice(logicalDevice)
	, mManagers(managers)
{
	createPipelines();
}

PipelineManager::~PipelineManager()
{
	std::map<std::string, Pipeline* >::iterator it;

	for (it = mIDToPipeline.begin(); it != mIDToPipeline.end(); ++it)
	{
		delete it->second;
	}
}

void PipelineManager::createPipelines()
{
	createSimplePipeline();
}

void PipelineManager::createSimplePipeline()
{
	Pipeline* pipeline = new Pipeline();

	pipeline->create(mLogicalDevice, mManagers);

	addPipeline("SimplePipeline", pipeline);
}

void PipelineManager::addPipeline(const std::string& ID, Pipeline* pipeline)
{
	mIDToPipeline[ID] = pipeline;
}

Pipeline* PipelineManager::getPipeline(const std::string& ID)
{
	std::map<std::string, Pipeline* >::iterator it = mIDToPipeline.find(ID);

	if (it != mIDToPipeline.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}