#pragma once
#include "RenderPass.h"

class Device;

class BeautyRenderPass : public RenderPass
{
public:
	BeautyRenderPass(Device* device, 
		PassInfo passinfo);
	virtual ~BeautyRenderPass();

public:
	virtual void buildPass();
	virtual void recordCommand();
};