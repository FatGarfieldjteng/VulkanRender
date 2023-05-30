#pragma once

#include "Scene.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class Mesh;
class Texture;
class PBRMaterial;

class SimpleScene : public Scene
{
public:
	SimpleScene(Device* device);
	virtual ~SimpleScene();

public:
	virtual void init();
	virtual void loadScene();
	virtual int getMeshCount();
	virtual Mesh* getMesh(int index);
	virtual int getTextureCount();
	virtual Texture* getTexture(int index);

private:
	void loadObjScene();
	void loadGLTFScene();
	void updateBBox();
	void createPBRDescriptorLayout();
private:
	std::vector<Mesh*> mMeshes;
	std::vector<Texture*> mTextures;
	std::vector< PBRMaterial*> mPBRMaterials;
	VkDescriptorSetLayout mPBRDescriptorSetLayout = VK_NULL_HANDLE;
};