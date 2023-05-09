#include "Managers.h"
#include "ShaderManager.h"
#include "PassManager.h"
#include "PipelineManager.h"
#include "FormatManager.h"
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

	if (mFormatManager)
	{
		delete mFormatManager;
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

	// vertex format manager
	mFormatManager = new FormatManager();
}