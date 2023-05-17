#include "ConstantBuffer.h"
#include "Device.h"
#include <stdexcept>

ConstantBuffer::ConstantBuffer(Device* device,
    unsigned int maxFramesInFligt)
    :mDevice(device),
    mMaxFramesInFligt(maxFramesInFligt)
{
    
}

ConstantBuffer::~ConstantBuffer()
{
    for (size_t i = 0; i < mMaxFramesInFligt; ++i) 
    {
        vkDestroyBuffer(mDevice->getLogicalDevice(), 
            mUniformBuffers[i], 
            nullptr);

        vkFreeMemory(mDevice->getLogicalDevice(), 
            mUniformBuffersMemory[i], 
            nullptr);
    }

    vkDestroyDescriptorSetLayout(mDevice->getLogicalDevice(), 
        mDescriptorSetLayout,
        nullptr);
}

void ConstantBuffer::init(VkDeviceSize bufferSize)
{   
    VkDescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorCount = 1;
    uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboBinding.pImmutableSamplers = nullptr;
    uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding bindings[2] = { uboBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 2;
    layoutInfo.pBindings = bindings;

    if (vkCreateDescriptorSetLayout(mDevice->getLogicalDevice(), 
        &layoutInfo, nullptr, 
        &mDescriptorSetLayout) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }


    mUniformBuffers.resize(mMaxFramesInFligt);
    mUniformBuffersMemory.resize(mMaxFramesInFligt);
    mMappedData.resize(mMaxFramesInFligt);

    for (size_t i = 0; i < mMaxFramesInFligt; i++)
    {
        mDevice->createBuffer(bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            mUniformBuffers[i],
            mUniformBuffersMemory[i]);

        vkMapMemory(mDevice->getLogicalDevice(), 
            mUniformBuffersMemory[i], 
            0, 
            bufferSize, 
            0, 
            &mMappedData[i]);
    }
}

void ConstantBuffer::update(uint32_t frameIndex, void* data, size_t size)
{
    std::memcpy(mMappedData[frameIndex], data, size);
}