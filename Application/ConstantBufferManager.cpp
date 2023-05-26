#include "ConstantBufferManager.h"
#include "ConstantBuffer.h"
#include "Device.h"
#include "Scene.h"
#include "Texture.h"

#include <stdexcept>

#define GLM_FORCE_RADIANS

// use depth range [0.0, 1.0] in projection matrix, this is Vulkan sepc
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

ConstantBufferManager::ConstantBufferManager(Device* device,
	Scene* scene,
	unsigned int maxFramesInFligt)
	: mDevice(device),
	mScene(scene),
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

	vkDestroyDescriptorPool(mDevice->getLogicalDevice(), mDescriptorPool, nullptr);
}

void ConstantBufferManager::createDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> poolSizes(2);

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(mMaxFramesInFligt);

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(mMaxFramesInFligt);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
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

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = mScene->getTexture(0)->mImageView;
		imageInfo.sampler = mScene->getTexture(0)->mSampler;

		std::vector<VkWriteDescriptorSet> writeDescriptors(2);

		writeDescriptors[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptors[0].dstSet = descriptorSets[i];
		writeDescriptors[0].dstBinding = 0;
		writeDescriptors[0].dstArrayElement = 0;
		writeDescriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptors[0].descriptorCount = 1;
		writeDescriptors[0].pBufferInfo = &bufferInfo;

		writeDescriptors[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptors[1].dstSet = descriptorSets[i];
		writeDescriptors[1].dstBinding = 1;
		writeDescriptors[1].dstArrayElement = 0;
		writeDescriptors[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptors[1].descriptorCount = 1;
		writeDescriptors[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(mDevice->getLogicalDevice(), 
			static_cast<uint32_t>(writeDescriptors.size()),
			writeDescriptors.data(),
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

std::vector<VkDescriptorSet>& ConstantBufferManager::getDescriptorSets(const std::string& ID)
{
	std::map<std::string, std::vector<VkDescriptorSet> >::iterator it = mIDToDescriptorSets.find(ID);

	if (it != mIDToDescriptorSets.end())
	{
		return  it->second;
	}
	else
	{
		throw std::runtime_error("wrong ID for DescriptorSets!");
	}
}