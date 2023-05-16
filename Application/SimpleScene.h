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
	virtual void loadScene();
	virtual Mesh* getMesh(int index);

private:
	
	std::vector<Mesh*> mMeshes;
};