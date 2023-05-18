#include "Managers.h"
#include "ShaderManager.h"
#include "PassManager.h"
#include "PipelineManager.h"
#include "FormatManager.h"
#include "ConstantBufferManager.h"
#include "SwapChain.h"
#include "DepthStencilBuffer.h"
#include "Device.h"

Managers::Managers(Device* mDevice, 
	SwapChain* swapChain, 
	DepthStencilBuffer* depthStencilBuffer,
	Scene* scene)
	:mDevice(mDevice)
	, mSwapChain(swapChain)
	, mDepthStencilBuffer(depthStencilBuffer)
	, mScene(scene)
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

	if (mConstantBufferManager)
	{
		delete mConstantBufferManager;
	}
}

void Managers::createManagers()
{
	// shadermanager
	mShaderManager = new ShaderManager(mDevice->getLogicalDevice());

	// passmanager
	mPassManager = new PassManager(mDevice->getLogicalDevice(), mSwapChain, mDepthStencilBuffer);

	// vertex format manager
	mFormatManager = new FormatManager();

	// constant buffer manager
	mConstantBufferManager = new ConstantBufferManager(mDevice, mScene);

	// pipelinemanager
	mPipelineManager = new PipelineManager(mDevice->getLogicalDevice(), this);
}