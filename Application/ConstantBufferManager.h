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

public:
    void createConstantBuffers();

    void updateWVPConstantBuffer(uint32_t frameIndex);

    void updateWVPCameraPosConstantBuffer(uint32_t frameIndex);

    ConstantBuffer* getConstantBuffer(const std::string& ID);
   
private:
    void createSimpleConstantBuffer();

    void createPBRConstantBuffer();

    void addConstantBuffer(const std::string& ID, ConstantBuffer* constantBuffer);

private:
    
    Device* mDevice = nullptr;
    Scene* mScene = nullptr;
    Camera* mCamera = nullptr;
    unsigned int mMaxFramesInFligt = 2;

    std::map<std::string, ConstantBuffer*> mIDToConstantBuffer;
    std::map < std::string, std::vector<VkDescriptorSet>> mIDToDescriptorSets;
};
