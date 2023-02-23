#pragma once

#include "GraphicsSystem.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

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

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

private:
    bool checkValidationLayerSupport();

private:

    VkInstance mVkInstance = nullptr;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    const uint32_t WIDTH = 1024;
    const uint32_t HEIGHT = 768;
    GLFWwindow* mWindow = nullptr;
};