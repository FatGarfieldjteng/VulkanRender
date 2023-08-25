#include "ShadowConstantBuffer.h"
#include "Device.h"
#include "VulkanHelper.h"
#include <stdexcept>

ShadowConstantBuffer::ShadowConstantBuffer(Device* device,
    Scene* scene,
    unsigned int maxFramesInFligt)
    :ConstantBuffer(device, scene, maxFramesInFligt)
{
    init();
}

ShadowConstantBuffer::~ShadowConstantBuffer()
{

}

void ShadowConstantBuffer::createDescriptorPool()
{
    VkDescriptorPoolSize poolSize;

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

void ShadowConstantBuffer::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorCount = 1;
    uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboBinding.pImmutableSamplers = nullptr;
    uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboBinding;

    if (vkCreateDescriptorSetLayout(mDevice->getLogicalDevice(),
        &layoutInfo, nullptr,
        &mDescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void ShadowConstantBuffer::createDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(mMaxFramesInFligt, mDescriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = mDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(mMaxFramesInFligt);
    allocInfo.pSetLayouts = layouts.data();
        
    mDescriptorSets.resize(mMaxFramesInFligt);

    if (vkAllocateDescriptorSets(mDevice->getLogicalDevice(),
        &allocInfo,
        mDescriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < mMaxFramesInFligt; ++i)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = mUniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(LightMVPConstantBuffer);

        VkWriteDescriptorSet writeDescriptor;
        writeDescriptor = VulkanHelper::bufferWriteDescriptorSet(mDescriptorSets[i], 0, &bufferInfo);

        vkUpdateDescriptorSets(mDevice->getLogicalDevice(),
            1,
            &writeDescriptor,
            0,
            nullptr);
    }
}

void ShadowConstantBuffer::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(LightMVPConstantBuffer);

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

void ShadowConstantBuffer::update(uint32_t frameIndex, void* data, size_t size)
{
    std::memcpy(mMappedData[frameIndex], data, size);
}