#include "perspectiveCamera.h"


PerpectiveCamera::PerpectiveCamera(
	float fovy,
	float aspect,
	float near,
	float far,
	glm::vec3 Position,
	glm::vec3 Up,
	glm::vec3 Right
) :Camera(Position, Up, Right) {
	mFovy = fovy;
	mAspect = aspect;
	mNear = near;
	mFar = far;
}
PerpectiveCamera::~PerpectiveCamera()
{

}

glm::mat4 PerpectiveCamera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}