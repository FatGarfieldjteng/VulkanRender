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