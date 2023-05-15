#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

class Device;
class ConstantBuffer;

class ConstantBufferManager
{
public:

    ConstantBufferManager(Device* device,
        unsigned int maxFramesInFligt = 2);

    ~ConstantBufferManager();

    // WVP constant buffer
    struct MVPConstantBuffer
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };

public:
    void createConstantBuffers();

    void updateWVPConstantBuffer(uint32_t frameIndex, float timePassed, VkExtent2D extent);

    void addConstantBuffer(const std::string& ID, ConstantBuffer* constantBuffer);
    
    ConstantBuffer* getConstantBuffer(const std::string& ID);
    
private:
    void createWVPConstantBuffer();

    void createWVPDescriptorSets();

    void createDescriptorPool();

private:
    
    Device* mDevice = nullptr;
    VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    unsigned int mMaxFramesInFligt = 2;
    std::map<std::string, ConstantBuffer*> mIDToConstantBuffer;
};
