#pragma once
#include "RenderPass.h"

class Device;

class ShadowRenderPass : public RenderPass
{
public:
	ShadowRenderPass(Device* device);
	virtual ~ShadowRenderPass();
};