#include "TransitResourceManager.h"
#include "TransitDepthMap.h"

#include <stdexcept>

TransitResourceManager::TransitResourceManager(Device* device)
	:mDevice(device)
{
    createResources();
}

TransitResourceManager::~TransitResourceManager()
{
    std::map<std::string, TransitResource*>::iterator it;

	for (it = mIDToResource.begin(); it != mIDToResource.end(); ++it)
	{
        delete it->second;
	}
}

void TransitResourceManager::createResources()
{
	createTransitDepthMap();
}

void TransitResourceManager::createTransitDepthMap()
{
	TransitResource* resource = new TransitDepthMap(mDevice);
	


    addResource("ShadowPass", resource);
}

void TransitResourceManager::addResource(const std::string& ID, TransitResource* resource)
{
    mIDToResource[ID] = resource;
}

TransitResource* TransitResourceManager::getResource(const std::string& ID)
{
    std::map<std::string, TransitResource*>::iterator it = mIDToResource.find(ID);

	if (it != mIDToResource.end())
	{
		return  it->second;
	}
	else
	{
		return VK_NULL_HANDLE;
	}
}