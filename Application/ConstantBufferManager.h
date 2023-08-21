#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>

class Device;
class Scene;
class Camera;
class ConstantBuffer;

class ConstantBufferManager
{
public:

    ConstantBufferManager(Device* device,
        Camera* camera,
        Scene* scene,
        unsigned int maxFramesInFligt = 2);

    ~ConstantBufferManager();

    // WVP constant buffer
    struct MVPConstantBuffer
    {
        glm::mat4 mvp;
    };

public:
    void createConstantBuffers();

    void updateWVPConstantBuffer(uint32_t frameIndex, float timePassed, VkExtent2D extent);

    ConstantBuffer* getConstantBuffer(const std::string& ID);

    std::vector<VkDescriptorSet>& getDescriptorSets(const std::string& ID);
   
private:
    void createSimpleUniform();

    void createPBRUniform();

    void createWVPConstantBuffer();

    void createWVPDescriptorSets();

    void createDescriptorPool();

    void addConstantBuffer(const std::string& ID, ConstantBuffer* constantBuffer);

    void addDescriptorSets(const std::string& ID, const std::vector<VkDescriptorSet>& descriptorSets);

private:
    
    Device* mDevice = nullptr;
    Scene* mScene = nullptr;
    Camera* mCamera = nullptr;
    VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    unsigned int mMaxFramesInFligt = 2;
    std::map<std::string, ConstantBuffer*> mIDToConstantBuffer;
    std::map < std::string, std::vector<VkDescriptorSet>> mIDToDescriptorSets;
};
