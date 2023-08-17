#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanHelper
{
public:
    VulkanHelper() {};
    virtual ~VulkanHelper() {};

public:
    static VkWriteDescriptorSet bufferWriteDescriptorSet(
        VkDescriptorSet dstSet,
        uint32_t    dstBinding,
        const VkDescriptorBufferInfo* pBufferInfo,
        uint32_t    dstArrayElement = 0,
        uint32_t    descriptorCount = 1)
    {
        struct VkWriteDescriptorSet res;

        res.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        res.pNext = nullptr;
        res.dstSet = dstSet;
        res.dstBinding = dstBinding;
        res.dstArrayElement = dstArrayElement;
        res.descriptorCount = descriptorCount;
        res.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        res.pImageInfo = nullptr;
        res.pBufferInfo = pBufferInfo;
        res.pTexelBufferView = nullptr;

        return res;
    }

    static VkWriteDescriptorSet imageSamplerWriteDescriptorSet(
        VkDescriptorSet dstSet,
        uint32_t    dstBinding,
        const VkDescriptorImageInfo* pImageInfo,
        uint32_t    dstArrayElement = 0,
        uint32_t    descriptorCount = 1)
    {
        struct VkWriteDescriptorSet res;

        res.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        res.pNext = nullptr;
        res.dstSet = dstSet;
        res.dstBinding = dstBinding;
        res.dstArrayElement = dstArrayElement;
        res.descriptorCount = descriptorCount;
        res.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        res.pImageInfo = pImageInfo;
        res.pBufferInfo = nullptr;
        res.pTexelBufferView = nullptr;

        return res;
    }
        
};      
