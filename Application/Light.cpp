#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"

Light::Light(bool castShadow)
	:mCastShadow(castShadow)
{
	create();
}

Light::~Light()
{
	if (mLightCamera)
	{
		delete mLightCamera;
	}
}

void Light::create()
{
	if (mCastShadow)
	{
		mLightCamera = new Camera();

		mLightCamera->setupPespective(90.0f, 1.0f, 0.001f, 100000.0f);
		mLightCamera->setupLookAt(mPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		mLightCamera->computeViewProj();
	}
}