#include "Managers.h"
#include "ShaderManager.h"
#include "PassManager.h"
#include "PipelineManager.h"
#include "SwapChain.h"

Managers::Managers(VkDevice logicalDevice, SwapChain* swapChain)
	:mLogicalDevice(logicalDevice)
	, mSwapChain(swapChain)
{
	createManagers();
}

Managers::~Managers()
{
	if (mPipelineManager)
	{
		delete mPipelineManager;
	}

	if (mPassManager)
	{
		delete mPassManager;
	}

	if (mShaderManager)
	{
		delete mShaderManager;
	}
}

void Managers::createManagers()
{
	// shadermanager
	mShaderManager = new ShaderManager(mLogicalDevice);

	// passmanager
	mPassManager = new PassManager(mLogicalDevice, mSwapChain);

	// pipelinemanager
	mPipelineManager = new PipelineManager(mLogicalDevice, this);
}