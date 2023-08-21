#include "ConstantBuffer.h"
#include "Device.h"
#include <stdexcept>

ConstantBuffer::ConstantBuffer(Device* device,
    Scene* scene,
    unsigned int maxFramesInFligt)
    :mDevice(device),
    mScene(scene),
    mMaxFramesInFligt(maxFramesInFligt)
{
    init();
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

    vkDestroyDescriptorPool(mDevice->getLogicalDevice(), mDescriptorPool, nullptr);
}

void ConstantBuffer::init()
{   
    createDescriptorPool();
    createDescriptorSetLayout();
    createDescriptorSets();
    createUniformBuffers();
}

void ConstantBuffer::update(uint32_t frameIndex, void* data, size_t size)
{
    std::memcpy(mMappedData[frameIndex], data, size);
}