#include "VertexBuffer.h"
#include "Device.h"

VertexBuffer::VertexBuffer(Device* device)
    :mDevice(device)
{
    
}

VertexBuffer::~VertexBuffer()
{
    vkDestroyBuffer(mDevice->getLogicalDevice(), 
        mVertexBuffer, 
        nullptr);

    vkFreeMemory(mDevice->getLogicalDevice(), 
        mVertexBufferMemory, 
        nullptr);
}

void VertexBuffer::init(uint32_t vertices, VkDeviceSize bufferSize, const void* bufferData)
{    
    mVertices = vertices;
    
    VkBuffer stagingVertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingVertexBufferMemory = VK_NULL_HANDLE;

    mDevice->createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingVertexBuffer,
        stagingVertexBufferMemory);

    void* data;
    vkMapMemory(mDevice->getLogicalDevice(),
        stagingVertexBufferMemory,
        0,
        bufferSize,
        0,
        &data);
    std::memcpy(data,
        bufferData,
        (size_t)bufferSize);
    vkUnmapMemory(mDevice->getLogicalDevice(),
        stagingVertexBufferMemory);


    mDevice->createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        mVertexBuffer,
        mVertexBufferMemory);

    mDevice->copyBuffer(stagingVertexBuffer, mVertexBuffer, bufferSize);

    vkDestroyBuffer(mDevice->getLogicalDevice(), stagingVertexBuffer, nullptr);
    vkFreeMemory(mDevice->getLogicalDevice(), stagingVertexBufferMemory, nullptr);
}