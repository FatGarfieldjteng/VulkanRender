#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>

class SwapChain;

class PassManager
{
public:

    PassManager(VkDevice logicalDevice, SwapChain* swapChain);

    ~PassManager();

public:
    void createPasses();

    void addPass(const std::string& ID, VkRenderPass pass);
    
    VkRenderPass getPass(const std::string& ID);
    

private:
    void createSimplePass();
    
private:
    
    VkDevice mLogicalDevice;
    SwapChain* mSwapChain;
    std::map<std::string, VkRenderPass> mIDToPass;
};
