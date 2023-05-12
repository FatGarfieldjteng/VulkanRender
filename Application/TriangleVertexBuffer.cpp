#include "TriangleVertexBuffer.h"
#include "PCVertexFormat.h"
#include "Device.h"

#include <vector>
#include <stdexcept>

TriangleVertexBuffer::TriangleVertexBuffer(Device* device)
    :VertexBuffer(device)
{
    init();
}

TriangleVertexBuffer::~TriangleVertexBuffer()
{
}

void TriangleVertexBuffer::init(uint32_t /*vertices*/, VkDeviceSize /*bufferSize*/, void* /*bufferData*/)
{
    // constant data, this is a triangle
    const std::vector<PCVertexFormat::Vertex> vertices = 
    {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    mVertices = static_cast<uint32_t>(vertices.size());

    VkDeviceSize bufferSize = sizeof(vertices[0]) * mVertices;
    
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
        vertices.data(), 
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