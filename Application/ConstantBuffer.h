#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Device;
class Scene;

class ConstantBuffer
{
public:
	ConstantBuffer(Device *device,
		Scene* scene,
		unsigned int maxFramesInFligt = 2);
	virtual ~ConstantBuffer();

public:
	virtual void init();

	VkDescriptorSetLayout* getDescriptorSetLayout()
	{
		return &mDescriptorSetLayout;
	}

	VkDescriptorSet* getDescriptorSets(size_t index)
	{
		return &(mDescriptorSets[index]);
	}

	void update(uint32_t frameIndex, void* data, size_t size);

protected:
	virtual void createDescriptorPool() = 0;
	virtual void createDescriptorSetLayout() = 0;
	virtual void createDescriptorSets() = 0;
	virtual void createUniformBuffers() = 0;
	
	Device* mDevice = nullptr;
	Scene* mScene = nullptr;
	
	unsigned int mMaxFramesInFligt = 2;
	VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;

	std::vector<VkBuffer> mUniformBuffers;
	std::vector<VkDeviceMemory> mUniformBuffersMemory;
	std::vector<void*> mMappedData;
	std::vector<VkDescriptorSet> mDescriptorSets;
};