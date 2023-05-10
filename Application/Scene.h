#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Mesh;

class Scene
{
public:
	Scene();
	~Scene();

public:
	virtual void init() = 0;
};