#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>

class SwapChain;
class DepthStencilBuffer;

class PassManager
{
public:

    PassManager(VkDevice logicalDevice, SwapChain* swapChain, DepthStencilBuffer* depthStencilBuffer);

    ~PassManager();

public:
    void createPasses();

    void addPass(const std::string& ID, VkRenderPass pass);
    
    VkRenderPass getPass(const std::string& ID);
    

private:
    void createSimplePass();

    void createShadowPass();
    
private:
    
    VkDevice mLogicalDevice = VK_NULL_HANDLE;
    SwapChain* mSwapChain = nullptr;
    DepthStencilBuffer* mDepthStencilBuffer = nullptr;
    std::map<std::string, VkRenderPass> mIDToPass;
};
