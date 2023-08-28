#pragma once
#define GLFW_INCLUDE_VULKAN

#include "TransitResource.h"
#include <GLFW/glfw3.h>

#include <string>

class Device;

class TransitDepthMap : public TransitResource
{
public:
	TransitDepthMap(Device *device,
		unsigned int maxFramesInFligt = 2);
	virtual ~TransitDepthMap();

protected:
	virtual void create();
};