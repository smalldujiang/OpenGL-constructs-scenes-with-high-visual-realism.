#pragma once

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl
{
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	//父类中，需要重写的函数
	void onCursor(double xpos, double ypos)override;
	

private:
//供自己调用的工具函数
	////02、分开 pitch 和 yaw 各自计算
	void pitch(float angle1, float angle2);
	//void yaw(float angle);
};

