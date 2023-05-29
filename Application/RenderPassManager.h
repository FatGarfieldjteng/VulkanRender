#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>

class RenderPass;
class Device;

class RenderPassManager
{
public:

    RenderPassManager(Device* device);

    ~RenderPassManager();

public:
    void createPasses();

    void addPass(const std::string& ID, RenderPass* pass);
    
    RenderPass* getPass(const std::string& ID);

private:
    void createClearRenderPass();
    void createBeautyRenderPass();
    void createFinalRenderPass();
    
private:
    Device* mDevice = nullptr;

    std::map<std::string, RenderPass*> mIDToRenderPass;
};
