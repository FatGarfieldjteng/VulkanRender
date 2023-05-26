#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

public:
	void setupLookAt(const glm::vec3& eye,
	const glm::vec3 center,
	const glm::vec3 up);

	void setupPespective(float FOV, 
		float aspect, 
		float near, 
		float far);

	void computeView();
	void computeProj();
	void computeViewProj();

private:
	
	glm::vec3 mEye;
	glm::vec3 mCenter;
	glm::vec3 mUp;

	float mFOV = 45.0f;
	float mAspect = 1.0f;
	float mNear = 0.01f;
	float mFar = 100000.0f;

	glm::mat4 mView;
	glm::mat4 mProj;
	glm::mat4 mViewProj;
};