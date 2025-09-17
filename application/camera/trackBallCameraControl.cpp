#pragma once
#include "trackBallCameraControl.h"

TrackBallCameraControl::TrackBallCameraControl()
{
}

TrackBallCameraControl::~TrackBallCameraControl()
{
}


void TrackBallCameraControl::onCursor(double xpos, double ypos) {

	if (mLeftMouseDown) {
	
		float deltaX = (xpos - mCurrentX) * mSensitivity;
		float deltaY = (ypos - mCurrentY) * mSensitivity;

		if (deltaY > 89.0f)
			deltaY = 89.0f;
		if (deltaY < -89.0f)
			deltaY = -89.0f;
		
		pitch(-deltaY , -deltaX);
		//yaw(-deltaX);
		
	}

	
	mCurrentX = xpos;
	mCurrentX = ypos;
}

//·â×°pitch½ÇÐý×ªº¯Êý
void TrackBallCameraControl::pitch(float angle1, float angle2) {

	auto mat1 = glm::rotate(glm::mat4(1.0f), glm::radians(angle1), mCamera->mRight);
	auto mat2 = glm::rotate(glm::mat4(1.0f), glm::radians(angle2), mCamera->mUp);

	auto mat = mat1 * mat2;
	
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);

	mCamera->mFront = glm::cross(mCamera->mUp, mCamera->mRight);

	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}



