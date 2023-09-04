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
	
	ClearRenderPass* pass = new ClearRenderPass(mDevice);

	addPass("clear", pass);
}

void RenderPassManager::createBeautyRenderPass()
{
	BeautyRenderPass *pass = new BeautyRenderPass(mDevice);

	addPass("beauty", pass);
}

void RenderPassManager::createShadowRenderPass()
{
	ShadowRenderPass* pass = new ShadowRenderPass(mDevice);

	addPass("shadow", pass);
}

void RenderPassManager::createFinalRenderPass()
{
	FinalRenderPass* pass = new FinalRenderPass(mDevice);

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