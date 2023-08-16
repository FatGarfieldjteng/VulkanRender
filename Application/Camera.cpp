#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

	mForward = glm::normalize(center - eye);
	mRight = glm::normalize(glm::cross(up, mForward));
	mUp = glm::normalize(glm::cross(mForward, mRight));

	computeView();
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
	computeProj();
}

void Camera::setCameraMoveSpeed(glm::vec3 moveSpeed)
{
	mMoveSpeed = moveSpeed;
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

void Camera::moveLeft()
{
	mEye -= mRight * mMoveSpeed.y;
	mCenter = mEye + mForward;

	computeView(); 
	computeViewProj();
}

void Camera::moveRight()
{
	mEye += mRight * mMoveSpeed.y;
	mCenter = mEye + mForward;

	computeView();
	computeViewProj();
}

void Camera::moveForward()
{
	mEye += mForward * mMoveSpeed.z;
	mCenter = mEye + mForward;

	computeView();
	computeViewProj();
}

void Camera::moveBackward()
{
	mEye -= mForward * mMoveSpeed.z;
	mCenter = mEye + mForward;

	computeView();
	computeViewProj();
}

void Camera::pitch(float direction)
{
	// frame rotate around up axis
	mRight = glm::rotate(mRight, mRotateSpeed.y * direction, mUp);
	mForward = glm::cross(mRight, mUp);
	mCenter = mEye + mForward;

	computeView();
	computeViewProj();
}


void Camera::yaw(float direction)
{
	// frame rotate around right axis
	mForward = glm::rotate(mForward, mRotateSpeed.x * direction, mRight);
	mUp = glm::cross(mForward, mRight);
	mCenter = mEye + mForward;

	computeView();
	computeViewProj();
}