#pragma once

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl
{
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	//�����У���Ҫ��д�ĺ���
	void onCursor(double xpos, double ypos)override;
	

private:
//���Լ����õĹ��ߺ���
	////02���ֿ� pitch �� yaw ���Լ���
	void pitch(float angle1, float angle2);
	//void yaw(float angle);
};

