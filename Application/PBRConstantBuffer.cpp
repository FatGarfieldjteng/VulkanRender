#include "PBRConstantBuffer.h"
#include "Device.h"
#include "Scene.h"
#include "Texture.h"
#include <stdexcept>

PBRConstantBuffer::PBRConstantBuffer(Device* device,
    Scene* scene,
    unsigned int maxFramesInFligt)
    :ConstantBuffer(device, scene, maxFramesInFligt)
{
    
}

PBRConstantBuffer::~PBRConstantBuffer()
{

}

void PBRConstantBuffer::update(uint32_t frameIndex, void* data, size_t size)
{

}

void PBRConstantBuffer::createDescriptorPool()
{

}

void PBRConstantBuffer::createDescriptorSetLayout()
{

}

void PBRConstantBuffer::createDescriptorSets()
{

}

void PBRConstantBuffer::createUniformBuffers()
{

}