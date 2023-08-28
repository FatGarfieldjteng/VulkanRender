#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class SwapChain;
class DepthStencilBuffer;
class ShaderManager;
class PassManager;
class PipelineManager;
class FormatManager;
class ConstantBufferManager;
class RenderPassManager;
class TransitResourceManager;
class Device;
class Scene;
class Camera;

class Managers
{
public:

    Managers(Device* device, 
        SwapChain* swapChain, 
        DepthStencilBuffer* depthStencilBuffer,
        Camera* camera,
        Scene* scene);

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

    RenderPassManager* getRenderPassManager()
    {
        return mRenderPassManager;
    }

    TransitResourceManager* getTransitResourceManager()
    {
        return mTransitResourceManager;
    }

private:
    void createManagers();

private:
    Device* mDevice = nullptr;
    SwapChain* mSwapChain = nullptr;
    DepthStencilBuffer* mDepthStencilBuffer = nullptr;
    Scene* mScene = nullptr;
    Camera* mCamera = nullptr;

    ShaderManager*          mShaderManager = nullptr;
    PassManager*            mPassManager = nullptr;
    PipelineManager*        mPipelineManager = nullptr;
    FormatManager*          mFormatManager = nullptr;
    ConstantBufferManager*  mConstantBufferManager = nullptr;
    RenderPassManager*      mRenderPassManager = nullptr;
    TransitResourceManager* mTransitResourceManager = nullptr;
};
