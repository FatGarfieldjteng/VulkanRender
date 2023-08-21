#pragma once

#include "Scene.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

class Mesh;
class Texture;
class PBRMaterial;
class CubeTexture;
class GLITexture;

struct MVPCameraPosConstantBuffer
{
	glm::mat4 mvp;
	glm::vec4 cameraPos;
};

class SimpleScene : public Scene
{
public:
	SimpleScene(Device* device);
	virtual ~SimpleScene();

public:
	virtual void init();
	virtual void createBox();
	virtual void loadScene();
	virtual int getMeshCount();
	virtual Mesh* getMesh(int index);
	virtual int getTextureCount();
	virtual Texture* getTexture(int index);
	
	// get functions

	// get material
	const std::vector< PBRMaterial*>& getPBRMaterials()
	{
		return mPBRMaterials;
	}

	Texture* getDefaultTexture()
	{
		return mDefaultTexture;
	}

	CubeTexture* getEnvTexture()
	{
		return mEnvTexture;
	}

	CubeTexture* getEnvIrrTexture()
	{
		return mEnvIrrTexture;
	}

	GLITexture* getEnvLUTTexture()
	{
		return mLUTTexture;
	}

private:
	void loadObjScene();
	void loadGLTFScene();
	void loadIBLTextures();
	void loadDefaultTexture();
	void updateBBox();

private:
	std::vector<Mesh*> mMeshes;
	std::vector<Texture*> mTextures;
	Texture* mDefaultTexture;
	CubeTexture* mEnvTexture;
	CubeTexture* mEnvIrrTexture;
	GLITexture* mLUTTexture;
	std::vector< PBRMaterial*> mPBRMaterials;
	VkDescriptorSetLayout mPBRDescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
	std::vector<VkDescriptorSet> mDescriptorSets;

	std::vector<VkBuffer> mUniformBuffers;
	std::vector<VkDeviceMemory> mUniformBuffersMemory;
	std::vector<void*> mMappedData;
};