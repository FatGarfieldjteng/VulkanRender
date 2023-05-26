#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::setupLookAt(const glm::vec3& eye,
	const glm::vec3 center,
	const glm::vec3 up)
{
	mEye = eye;
	mCenter = center;
	mUp = up;
}

void Camera::setupPespective(float FOV,
	float aspect,
	float near,
	float far)
{
	mFOV = FOV;
	mAspect = aspect;
	mNear = near;
	mFar = far;
}

void Camera::computeView()
{
	mView = glm::lookAt(mEye, mCenter, mUp);
}

void Camera::computeProj()
{
	mProj = glm::perspective(mFOV, mAspect, mNear, mFar);
	mProj[1][1] *= -1;
}

void Camera::computeViewProj()
{
	mViewProj = mProj * mView;
}