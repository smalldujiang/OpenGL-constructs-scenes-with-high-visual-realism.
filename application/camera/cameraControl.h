#pragma once

#include "../../glframework/core.h"
#include "camera.h"
#include <map>//储存按键用

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	//函数接口
	virtual void onMouse(int button, int action, double xpos, double ypos);
	virtual void onCursor(double xpos, double ypos);
	virtual void onKey(int key, int action, int mods);

	//每帧更新
	virtual void update();
	void setCamera(Camera* camera) { mCamera = camera; }
	void setSensitity(float s) { mSensitivity = s; }

protected://使用受保护；方便派生类使用
	//01鼠标按键
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;

	//02鼠标位置
	float mCurrentX = 0.0f, mCurrentY = 0.0f;

	//03敏感度
	float mSensitivity = 0.002f;
	float mMovementSpeed = 0.2f;

	//04记录键盘相关按键的按下状态
	std::map<int, bool> mKeyMap;

	//存储当前控制的摄像机
	Camera* mCamera = nullptr;
};
