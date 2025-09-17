#pragma once
#include "camera.h"

class PerpectiveCamera :public Camera
{

public:
	PerpectiveCamera(
		float fovy,
		float aspect,
		float near,
		float far,
		glm::vec3 Position,
		glm::vec3 Up,
		glm::vec3 Right
	);
	~PerpectiveCamera();

	glm::mat4 getProjectionMatrix()override;

private:
	float mFovy = 0.0f;
	float mAspect = 0.0f;
	
};