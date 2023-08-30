#pragma once
#include <glm/glm.hpp>

class Camera;

class Light
{
public:
	Light(bool castShadow);
	virtual ~Light();

public:
	void create();

	Camera* getLightCamera()
	{
		return mLightCamera;
	}

protected:

	bool mCastShadow = false;

	glm::vec3 mPos = glm::vec3(5.0f, 15.0f, 5.0f);

	Camera* mLightCamera = nullptr;
};