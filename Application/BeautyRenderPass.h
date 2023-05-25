#pragma once
#include "RenderPass.h"

class Device;

class BeautyRenderPass : public RenderPass
{
public:
	BeautyRenderPass(Device* device);
	virtual ~BeautyRenderPass();
};