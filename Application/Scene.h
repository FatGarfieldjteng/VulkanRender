#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Mesh;
class Device;

class Scene
{
public:
	Scene(Device* device);
	virtual ~Scene();

public:
	virtual void init() = 0;
	virtual Mesh* getMesh(int index) = 0;

protected:
	Device* mDevice = nullptr;
};