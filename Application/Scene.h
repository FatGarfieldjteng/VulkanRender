#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class Mesh;
class Device;
class Texture;
class BoundingBox;

class Scene
{
public:
	Scene(Device* device);
	virtual ~Scene();

public:
	virtual void init() = 0;
	virtual void loadScene() = 0;
	virtual int getMeshCount() = 0;
	virtual int getMaterialCount() = 0;
	virtual Mesh* getMesh(int index) = 0;
	virtual int getTextureCount() = 0;
	virtual Texture* getTexture(int index) = 0;

public:
	BoundingBox* getBBox()
	{
		return mBBox;
	}

protected:
	Device* mDevice = nullptr;
	BoundingBox* mBBox;
};