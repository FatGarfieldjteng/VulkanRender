#include "RenderPassManager.h"
#include "Device.h"
#include "RenderPass.h"

#include <stdexcept>

RenderPassManager::RenderPassManager(Device* device)
	:mDevice(device)
{
	createPasses();
}

RenderPassManager::~RenderPassManager()
{

}

void RenderPassManager::createPasses()
{
	createSimplePass();
}

void RenderPassManager::createSimplePass()
{
   
}

void RenderPassManager::addPass(const std::string& ID, RenderPass* pass)
{
	mIDToPass[ID] = pass;
}

RenderPass* RenderPassManager::getPass(const std::string& ID)
{
	std::map<std::string, RenderPass* >::iterator it = mIDToPass.find(ID);

	if (it != mIDToPass.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}