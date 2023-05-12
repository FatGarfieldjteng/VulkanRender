#include "IndexBuffer.h"
#include "Device.h"

IndexBuffer::IndexBuffer(Device* device)
    :mDevice(device)
{
    
}

IndexBuffer::~IndexBuffer()
{
    vkDestroyBuffer(mDevice->getLogicalDevice(), 
        mIndexBuffer,
        nullptr);

    vkFreeMemory(mDevice->getLogicalDevice(), 
        mIndexBufferMemory,
        nullptr);
}

void IndexBuffer::init(uint32_t indices, VkDeviceSize bufferSize, void* bufferData)
{
    mIndices = indices;

    VkBuffer stagingIndexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingIndexBufferMemory = VK_NULL_HANDLE;

    mDevice->createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingIndexBuffer,
        stagingIndexBufferMemory);

    void* data;
    vkMapMemory(mDevice->getLogicalDevice(),
        stagingIndexBufferMemory,
        0,
        bufferSize,
        0,
        &data);
    std::memcpy(data,
        bufferData,
        (size_t)bufferSize);
    vkUnmapMemory(mDevice->getLogicalDevice(),
        stagingIndexBufferMemory);


    mDevice->createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        mIndexBuffer,
        mIndexBufferMemory);

    mDevice->copyBuffer(stagingIndexBuffer, mIndexBuffer, bufferSize);

    vkDestroyBuffer(mDevice->getLogicalDevice(), stagingIndexBuffer, nullptr);
    vkFreeMemory(mDevice->getLogicalDevice(), stagingIndexBufferMemory, nullptr);
}