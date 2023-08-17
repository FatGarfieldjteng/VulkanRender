#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

class VulkanHelper
{
public:
    VulkanHelper() {};
    virtual ~VulkanHelper() {};

public:
	static VkWriteDescriptorSet bufferWriteDescriptorSet(
        VkDescriptorSet dstSet,
        uint32_t    dstBinding,
        uint32_t    dstArrayElement,
        uint32_t    descriptorCount,
        VkDescriptorType    descriptorType,
        const VkDescriptorBufferInfo* pBufferInfo,
        const VkBufferView* pTexelBufferView)
	{
		VkWriteDescriptorSet res(0);



		return 
	}

};