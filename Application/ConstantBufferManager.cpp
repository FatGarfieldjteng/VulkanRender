#include "ConstantBufferManager.h"
#include "ConstantBuffer.h"
#include "Device.h"

#include <stdexcept>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

ConstantBufferManager::ConstantBufferManager(Device* device,
	unsigned int maxFramesInFligt)
	: mDevice(device),
	mMaxFramesInFligt(maxFramesInFligt)
	
{
	createDescriptorPool();
	createConstantBuffers();
}

ConstantBufferManager::~ConstantBufferManager()
{
	std::map<std::string, ConstantBuffer* >::iterator it;

	for (it = mIDToConstantBuffer.begin(); it != mIDToConstantBuffer.end(); ++it)
	{
		delete it->second;
	}
}

void ConstantBufferManager::createDescriptorPool()
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(mMaxFramesInFligt);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(mMaxFramesInFligt);

	if (vkCreateDescriptorPool(mDevice->getLogicalDevice(), 
		&poolInfo, 
		nullptr, 
		&mDescriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void ConstantBufferManager::createConstantBuffers()
{
	// WVP
	createWVPConstantBuffer();

	createWVPDescriptorSets();
}

void ConstantBufferManager::createWVPConstantBuffer()
{
	ConstantBuffer* constantBuffer = new ConstantBuffer(mDevice);

	VkDeviceSize bufferSize = sizeof(MVPConstantBuffer);

	constantBuffer->init(bufferSize);

	addConstantBuffer("WVP", constantBuffer);
}

void ConstantBufferManager::createWVPDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(mMaxFramesInFligt, getConstantBuffer("WVP")->mDescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = mDescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(mMaxFramesInFligt);
	allocInfo.pSetLayouts = layouts.data();

	std::vector<VkDescriptorSet> descriptorSets;
	descriptorSets.resize(mMaxFramesInFligt);

	if (vkAllocateDescriptorSets(mDevice->getLogicalDevice(), 
		&allocInfo, 
		descriptorSets.data()) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	ConstantBuffer* constantBuffer = getConstantBuffer("WVP");

	for (size_t i = 0; i < mMaxFramesInFligt; ++i) 
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = constantBuffer->mUniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(MVPConstantBuffer);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(mDevice->getLogicalDevice(), 
			1, 
			&descriptorWrite, 
			0, 
			nullptr);
	}

	addDescriptorSets("WVP", descriptorSets);
}

void ConstantBufferManager::updateWVPConstantBuffer(uint32_t frameIndex, float timePassed, VkExtent2D extent)
{
	MVPConstantBuffer WVP{};
	WVP.model = glm::rotate(glm::mat4(1.0f), timePassed * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	WVP.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	WVP.projection = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);
	WVP.projection[1][1] *= -1;

	getConstantBuffer("WVP")->update(frameIndex, &WVP, sizeof(WVP));
}

void ConstantBufferManager::addConstantBuffer(const std::string& ID, ConstantBuffer* constantBuffer)
{
	mIDToConstantBuffer[ID] = constantBuffer;
}

void ConstantBufferManager::addDescriptorSets(const std::string& ID, const std::vector<VkDescriptorSet>& descriptorSets)
{
	mIDToDescriptorSets[ID] = descriptorSets;
}

ConstantBuffer* ConstantBufferManager::getConstantBuffer(const std::string& ID)
{
	std::map<std::string, ConstantBuffer* >::iterator it = mIDToConstantBuffer.find(ID);

	if (it != mIDToConstantBuffer.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}