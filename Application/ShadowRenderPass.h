#pragma once
#include "RenderPass.h"

class Device;

class ShadowRenderPass : public RenderPass
{
public:
	ShadowRenderPass(Device* device, PassInfo* passinfo);
	virtual ~ShadowRenderPass();

public:
	virtual void buildPass();
	virtual void recordCommand();
};