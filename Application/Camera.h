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

	void setupPespectiveShadow(float FOV,
		float aspect,
		float near,
		float far);

	void setCameraMoveSpeed(glm::vec3 moveSpeed);

	void computeView();
	void computeProj();
	void computeProjShadow();
	void computeViewProj();

	// move camera
	void moveLeft();
	void moveRight();
	void moveForward();
	void moveBackward();

	// rotate camera

	// rotate around right vector
	void pitch(float direction);

	// rotate around up vector
	void yaw(float direction);

	glm::mat4 getViewProj()
	{
		return mViewProj;
	}

	glm::vec4 getCameraPos()
	{
		return glm::vec4(mEye, 1.0f);
	}

private:
	
	// initial parameter
	glm::vec3 mEye;
	glm::vec3 mCenter;

	// coordinate frame
	glm::vec3 mForward;
	glm::vec3 mRight;
	glm::vec3 mUp;

	// move speed
	glm::vec3 mMoveSpeed = glm::vec3(0.005f, 0.005f, 0.005f);

	// rotate speed
	glm::vec3 mRotateSpeed = glm::vec3(0.01f, 0.01f, 0.01f);

	float mFOV = 45.0f;
	float mAspect = 1.0f;
	float mNear = 0.01f;
	float mFar = 100000.0f;

	glm::mat4 mView;
	glm::mat4 mProj;
	glm::mat4 mViewProj;
		
};