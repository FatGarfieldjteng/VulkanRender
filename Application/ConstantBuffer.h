#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Device;

class ConstantBuffer
{
public:
	ConstantBuffer(Device *device,
		unsigned int maxFramesInFligt = 2);
	virtual ~ConstantBuffer();

public:
	virtual void init(VkDeviceSize bufferSize);

	void update(uint32_t frameIndex, void* data, size_t size);

	Device* mDevice = nullptr;
	unsigned int mMaxFramesInFligt = 2;
	
	VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;

	std::vector<VkBuffer> mUniformBuffers;
	std::vector<VkDeviceMemory> mUniformBuffersMemory;
	std::vector<void*> mMappedData;
};