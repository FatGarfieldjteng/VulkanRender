#include "SimpleConstantBuffer.h"
#include "Device.h"
#include "Scene.h"
#include "Texture.h"
#include <stdexcept>

SimpleConstantBuffer::SimpleConstantBuffer(Device* device,
    Scene* scene,
    unsigned int maxFramesInFligt)
    :ConstantBuffer(device, scene, maxFramesInFligt)
{
    init();
}

SimpleConstantBuffer::~SimpleConstantBuffer()
{

}

void SimpleConstantBuffer::createDescriptorPool()
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

void SimpleConstantBuffer::createDescriptorSetLayout()
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

    VkDescriptorSetLayoutBinding bindings[2] = { uboBinding, samplerLayoutBinding };

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
}

void SimpleConstantBuffer::createDescriptorSets()
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
        bufferInfo.range = sizeof(MVPConstantBuffer);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = mScene->getTexture(0)->mImageView;
        imageInfo.sampler = mScene->getTexture(0)->mSampler;

        std::vector<VkWriteDescriptorSet> writeDescriptors(2);

        writeDescriptors[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[0].dstSet = mDescriptorSets[i];
        writeDescriptors[0].dstBinding = 0;
        writeDescriptors[0].dstArrayElement = 0;
        writeDescriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptors[0].descriptorCount = 1;
        writeDescriptors[0].pBufferInfo = &bufferInfo;

        writeDescriptors[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptors[1].dstSet = mDescriptorSets[i];
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
}

void SimpleConstantBuffer::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(MVPConstantBuffer);

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

void SimpleConstantBuffer::update(uint32_t frameIndex, void* data, size_t size)
{

    std::memcpy(mMappedData[frameIndex], data, size);
}