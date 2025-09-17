#include "cameraControl.h"
#include <iostream>

CameraControl::CameraControl() {

}

CameraControl::~CameraControl() {

}

//函数接口
void CameraControl::onMouse(int button, int action, double xpos, double ypos)
 {
	std::cout << "onMouse触发" << std::endl;
	//01、判断当前按键是否按下
	bool pressed = action == GLFW_PRESS ? true : false;

	//02、按下时记录按键位置
	if (pressed) {
		mCurrentX = xpos;
		mCurrentY = ypos;
	}

	//03、根据按下的鼠标按键，更改按键记录的状态
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
}

void CameraControl::onCursor(double xpos, double ypos) {
	std::cout << "onCursor(鼠标移动)触发" << std::endl;
	//没有公共部分，由各自实现
}

void CameraControl::onKey(int key, int action, int mods) {
	std::cout << "onKey触发" << std::endl;
	//过滤repeat（按住）的情况
	if (action == GLFW_REPEAT) {
		return;
	}
	
	//01、判断当前按键是否按下
	bool pressed = action == GLFW_PRESS ? true : false;
	
	//02、记录在keyMap中
	mKeyMap[key] = pressed;

	
}

//每帧更新
void CameraControl::update() {
	//没有公共部分，由各自实现
	// 根据按键更新相机位置
	if (mKeyMap[GLFW_KEY_W]) {
		// W 键：向前移动
		mCamera->mPosition += mCamera->mFront * mMovementSpeed;
	}
	if (mKeyMap[GLFW_KEY_S]) {
		// S 键：向后移动
		mCamera->mPosition -= mCamera->mFront * mMovementSpeed;
	}
	if (mKeyMap[GLFW_KEY_A]) {
		// A 键：向左移动
		mCamera->mPosition -= mCamera->mRight * mMovementSpeed;
	}
	if (mKeyMap[GLFW_KEY_D]) {
		// D 键：向右移动
		mCamera->mPosition += mCamera->mRight * mMovementSpeed;
	}
}