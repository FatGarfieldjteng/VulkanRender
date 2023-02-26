#pragma once

#include "GraphicsSystem.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class Validation;
class Device;
class Queue;

class GraphicsSystem
{
public:

	GraphicsSystem();
	~GraphicsSystem();

public:
    void run();

private:

    // create Vulkan instance
    void createInstance();

    void createValidation();

    void createDevice();

    void createQueue();

    void initWindow();

    void initVulkan();

    void createDebugMessenger();

    void createSurface();

    void mainLoop();

    void cleanup();

private:
    
    std::vector<const char*> getGLFWRequiredExtensions();
        
private:

    Validation* mValidation = nullptr;

    VkInstance mVkInstance = nullptr;

    Device* mDevice = nullptr;

    Queue* mQueue = nullptr;

    VkSurfaceKHR mSurface = VK_NULL_HANDLE;

#ifdef NDEBUG
    const bool mEnableValidationLayers = false;
#else
    const bool mEnableValidationLayers = true;
#endif

    const uint32_t WIDTH = 1024;
    const uint32_t HEIGHT = 768;
    GLFWwindow* mWindow = nullptr;
};