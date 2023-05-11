#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <vector>

class Mesh;


class SimpleScene : public Scene
{
public:
	SimpleScene(Device* device);
	virtual ~SimpleScene();

public:
	virtual void init();

private:
	
	std::vector<Mesh*> meshes;
};