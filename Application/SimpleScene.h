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
	virtual void loadScene();
	virtual int getMeshCount();
	virtual Mesh* getMesh(int index);
	virtual int getTextureCount();
	virtual Texture* getTexture(int index);

private:
	void loadObjScene();
	void loadGLTFScene();
	void loadIBLTextures();
	void updateBBox();
	void createUniformBuffers(int frameInFlight = 2);
	void createPBRDescriptorLayout();
	void createDescriptorPool(int materialCount, int uniformBufferCount, int samplerCount, int frameInFlight = 2);
	void createDescriptorSet(int frameInFlight = 2);
	
	
private:
	std::vector<Mesh*> mMeshes;
	std::vector<Texture*> mTextures;
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