#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SwapChain;
class ShaderManager;
class PassManager;
class PipelineManager;
class FormatManager;
class ConstantBufferManager;
class Device;

class Managers
{
public:

    Managers(Device* device, SwapChain* swapChain);

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

    FormatManager* getFormatManager()
    {
        return mFormatManager;
    }

    ConstantBufferManager* getConstantBufferManager()
    {
        return mConstantBufferManager;
    }

private:
    void createManagers();

private:
    Device* mDevice = nullptr;
    SwapChain* mSwapChain = nullptr;

    ShaderManager*      mShaderManager = nullptr;
    PassManager*        mPassManager = nullptr;
    PipelineManager*    mPipelineManager = nullptr;
    FormatManager*      mFormatManager = nullptr;
    ConstantBufferManager* mConstantBufferManager = nullptr;
};
