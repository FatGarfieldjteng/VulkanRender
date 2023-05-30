#pragma once

#define GLFW_INCLUDE_VULKAN
#include "RenderPass.h"

class Device;
class Scene;

class ClearRenderPass: public RenderPass
{
public:
	ClearRenderPass(Device* device,
		RenderPass::PassInfo passinfo);
	virtual ~ClearRenderPass();

public:
	
	virtual void buildPass();
	virtual void recordCommand();
		
};