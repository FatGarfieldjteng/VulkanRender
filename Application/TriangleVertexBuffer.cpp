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

void TriangleVertexBuffer::init()
{
    const std::vector<PCVertexFormat::Vertex> vertices = 
    {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    mSize = static_cast<uint32_t>(vertices.size());

    // create vertex buffer object
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * mSize;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(mDevice->getLogicalDevice(), 
        &bufferInfo, 
        nullptr, 
        &mVertexBuffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    // memory requirement
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(mDevice->getLogicalDevice(), 
        mVertexBuffer, 
        &memRequirements);

    uint32_t memoryTypeIndex = mDevice->findMemoryType(memRequirements.memoryTypeBits, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    if (vkAllocateMemory(mDevice->getLogicalDevice(), 
        &allocInfo, 
        nullptr, 
        &mVertexBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(mDevice->getLogicalDevice(), 
        mVertexBuffer, 
        mVertexBufferMemory, 0);

    void* data;
    vkMapMemory(mDevice->getLogicalDevice(), 
        mVertexBufferMemory, 
        0, 
        bufferInfo.size, 
        0, 
        &data);
    std::memcpy(data, 
        vertices.data(), 
        (size_t)bufferInfo.size);
    vkUnmapMemory(mDevice->getLogicalDevice(), 
        mVertexBufferMemory);
}