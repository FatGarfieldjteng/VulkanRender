#include "ConstantBufferManager.h"
#include "SimpleConstantBuffer.h"
#include "PBRConstantBuffer.h"
#include "ShadowConstantBuffer.h"
#include "Device.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"

#include <stdexcept>

#define GLM_FORCE_RADIANS

// use depth range [0.0, 1.0] in projection matrix, this is Vulkan sepc
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

ConstantBufferManager::ConstantBufferManager(Device* device,
	Camera* camera,
	Scene* scene,
	unsigned int maxFramesInFligt)
	: mDevice(device),
	mScene(scene),
	mCamera(camera),
	mMaxFramesInFligt(maxFramesInFligt)
	
{
	createConstantBuffers();
}

ConstantBufferManager::~ConstantBufferManager()
{
	std::map<std::string, ConstantBuffer* >::iterator it;

	for (it = mIDToConstantBuffer.begin(); it != mIDToConstantBuffer.end(); ++it)
	{
		delete it->second;
	}
	
}

void ConstantBufferManager::createConstantBuffers()
{
	//createSimpleConstantBuffer();
	createPBRConstantBuffer();

	createShadowConstantBuffer();
}

void ConstantBufferManager::createPBRConstantBuffer()
{
	ConstantBuffer* constantBuffer = new PBRConstantBuffer(mDevice,
		mScene);
	addConstantBuffer("PBR", constantBuffer);
}

void ConstantBufferManager::createSimpleConstantBuffer()
{
	ConstantBuffer* constantBuffer = new SimpleConstantBuffer(mDevice,
		mScene);
	addConstantBuffer("Simple", constantBuffer);
}

void ConstantBufferManager::createShadowConstantBuffer()
{
	ConstantBuffer* constantBuffer = new ShadowConstantBuffer(mDevice,
		mScene);
	addConstantBuffer("Shadow", constantBuffer);
}

void ConstantBufferManager::updateWVPConstantBuffer(uint32_t frameIndex)
{
	SimpleConstantBuffer::MVPConstantBuffer WVP{};
	
	WVP.mvp = mCamera->getViewProj();

	getConstantBuffer("Simple")->update(frameIndex, &WVP, sizeof(WVP));
}

void ConstantBufferManager::updateWVPCameraPosConstantBuffer(uint32_t frameIndex)
{
	PBRConstantBuffer::MVPCameraPosConstantBuffer WVPCameraPos{};

	WVPCameraPos.mvp = mCamera->getViewProj();
	WVPCameraPos.cameraPos = mCamera->getCameraPos();

	uint32_t index = frameIndex * 2;
	getConstantBuffer("PBR")->update(index, &WVPCameraPos, sizeof(WVPCameraPos));
}

void ConstantBufferManager::updateShadowConstantBuffer(uint32_t frameIndex)
{
	ShadowConstantBuffer::LightMVPConstantBuffer lightMVP{};

	lightMVP.mvp = mDevice->getLight()->getLightCamera()->getViewProj();

	getConstantBuffer("Shadow")->update(frameIndex, &lightMVP, sizeof(lightMVP));
}

void ConstantBufferManager::updatePBRShadowConstantBuffer(uint32_t frameIndex)
{
	ShadowConstantBuffer::LightMVPConstantBuffer lightMVP{};

	lightMVP.mvp = mDevice->getLight()->getLightCamera()->getViewProj();

	uint32_t index = frameIndex * 2 + 1;
	getConstantBuffer("PBR")->update(index, &lightMVP, sizeof(lightMVP));
}


void ConstantBufferManager::addConstantBuffer(const std::string& ID, ConstantBuffer* constantBuffer)
{
	mIDToConstantBuffer[ID] = constantBuffer;
}

ConstantBuffer* ConstantBufferManager::getConstantBuffer(const std::string& ID)
{
	std::map<std::string, ConstantBuffer* >::iterator it = mIDToConstantBuffer.find(ID);

	if (it != mIDToConstantBuffer.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}