#include "Managers.h"
#include "ShaderManager.h"
#include "PassManager.h"
#include "PipelineManager.h"
#include "FormatManager.h"
#include "ConstantBufferManager.h"
#include "RenderPassManager.h"
#include "SwapChain.h"
#include "DepthStencilBuffer.h"
#include "Device.h"

Managers::Managers(Device* mDevice, 
	SwapChain* swapChain, 
	DepthStencilBuffer* depthStencilBuffer,
	Camera* camera,
	Scene* scene)
	:mDevice(mDevice)
	, mSwapChain(swapChain)
	, mDepthStencilBuffer(depthStencilBuffer)
	, mCamera(camera)
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

	if (mRenderPassManager)
	{
		delete mRenderPassManager;
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
	mConstantBufferManager = new ConstantBufferManager(mDevice, mCamera, mScene);

	// pipelinemanager
	mPipelineManager = new PipelineManager(mDevice->getLogicalDevice(), this);

	//renderpassmanager, manage Vulkan renderpass
	mRenderPassManager = new RenderPassManager(mDevice);
}