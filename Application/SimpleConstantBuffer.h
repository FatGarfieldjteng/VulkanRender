#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "ConstantBuffer.h"

class Device;

class SimpleConstantBuffer: public ConstantBuffer
{
public:
	SimpleConstantBuffer(Device *device,
		Scene* scene,
		unsigned int maxFramesInFligt = 2);
	virtual ~SimpleConstantBuffer();

	// WVP constant buffer
	struct MVPConstantBuffer
	{
		glm::mat4 mvp;
	};

public:
	
	void update(uint32_t frameIndex, void* data, size_t size);

protected:
	virtual void createDescriptorPool();
	virtual void createDescriptorSetLayout();
	virtual void createDescriptorSets();
	virtual void createUniformBuffers();
};