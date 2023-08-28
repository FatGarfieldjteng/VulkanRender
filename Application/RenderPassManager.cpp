#include "RenderPassManager.h"
#include "Device.h"
#include "RenderPass.h"
#include "ClearRenderPass.h"
#include "BeautyRenderPass.h"
#include "FinalRenderPass.h"
#include "ShadowRenderPass.h"

#include <stdexcept>

RenderPassManager::RenderPassManager(Device* device)
	:mDevice(device)
{
	createPasses();
}

RenderPassManager::~RenderPassManager()
{
	std::map<std::string, RenderPass* >::iterator it;

	for (it = mIDToRenderPass.begin(); it != mIDToRenderPass.end(); ++it)
	{
		delete it->second;
	}
}

void RenderPassManager::createPasses()
{
	createClearRenderPass();
	createBeautyRenderPass();
	createFinalRenderPass();
	createShadowRenderPass();
}	

void RenderPassManager::createClearRenderPass()
{
	RenderPass::PassInfo passInfo;
	passInfo.isFirstPass = true;

	ClearRenderPass* pass = new ClearRenderPass(mDevice, passInfo);

	addPass("clear", pass);
}

void RenderPassManager::createBeautyRenderPass()
{
	RenderPass::PassInfo passInfo;
	
	BeautyRenderPass *pass = new BeautyRenderPass(mDevice, passInfo);

	addPass("beauty", pass);
}

void RenderPassManager::createShadowRenderPass()
{
	RenderPass::PassInfo passInfo;

	ShadowRenderPass* pass = new ShadowRenderPass(mDevice, passInfo);

	addPass("shadow", pass);
}

void RenderPassManager::createFinalRenderPass()
{
	RenderPass::PassInfo passInfo;
	passInfo.isLastPass = true;

	FinalRenderPass* pass = new FinalRenderPass(mDevice, passInfo);

	addPass("final", pass);

}

void RenderPassManager::addPass(const std::string& ID, RenderPass* pass)
{
	mIDToRenderPass[ID] = pass;
}

RenderPass* RenderPassManager::getPass(const std::string& ID)
{
	std::map<std::string, RenderPass* >::iterator it = mIDToRenderPass.find(ID);

	if (it != mIDToRenderPass.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}