#pragma once

#include "GraphicsSystem.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
class GraphicsSystem
{
public:

	GraphicsSystem();
	~GraphicsSystem();

public:
    void run();

private:
    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

private:
    const uint32_t WIDTH = 1024;
    const uint32_t HEIGHT = 768;
    GLFWwindow* mWindow = nullptr;
};