#include "PipelineManager.h"
#include "SimplePipeline.h"
#include "PBRPipeline.h"
#include "Scene.h"

PipelineManager::PipelineManager(VkDevice logicalDevice, 
	Scene* scene,
	Managers* managers)
	:mLogicalDevice(logicalDevice)
	, mScene(scene)
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
	//createSimplePipeline();

	createPBRPipeline();
}

void PipelineManager::createSimplePipeline()
{
	Pipeline* pipeline = new SimplePipeline(mLogicalDevice, mScene, mManagers);

	pipeline->create();

	addPipeline("Simple", pipeline);
}

void PipelineManager::createPBRPipeline()
{
	Pipeline* pipeline = new PBRPipeline(mLogicalDevice, mScene, mManagers);

	pipeline->create();

	addPipeline("PBR", pipeline);
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