#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SwapChain;
class ShaderManager;
class PassManager;
class PipelineManager;

class Managers
{
public:

    Managers(VkDevice logicalDevice, SwapChain* swapChain);

    ~Managers();

public:
    ShaderManager* getShaderManager()
    {
        return mShaderManager;
    }

    PassManager* getPassManager()
    {
        return mPassManager;
    }

    PipelineManager* getPipelineManager()
    {
        return mPipelineManager;
    }

private:
    void createManagers();

private:
    VkDevice mLogicalDevice = VK_NULL_HANDLE;
    SwapChain* mSwapChain = nullptr;

    ShaderManager*      mShaderManager = nullptr;
    PassManager*        mPassManager = nullptr;
    PipelineManager*    mPipelineManager = nullptr;
};
