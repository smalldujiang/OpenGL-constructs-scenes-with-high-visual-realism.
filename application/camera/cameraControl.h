#pragma once

#include "../../glframework/core.h"
#include "camera.h"
#include <map>//���水����

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	//�����ӿ�
	virtual void onMouse(int button, int action, double xpos, double ypos);
	virtual void onCursor(double xpos, double ypos);
	virtual void onKey(int key, int action, int mods);

	//ÿ֡����
	virtual void update();
	void setCamera(Camera* camera) { mCamera = camera; }
	void setSensitity(float s) { mSensitivity = s; }

protected://ʹ���ܱ���������������ʹ��
	//01��갴��
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;

	//02���λ��
	float mCurrentX = 0.0f, mCurrentY = 0.0f;

	//03���ж�
	float mSensitivity = 0.002f;
	float mMovementSpeed = 0.2f;

	//04��¼������ذ����İ���״̬
	std::map<int, bool> mKeyMap;

	//�洢��ǰ���Ƶ������
	Camera* mCamera = nullptr;
};
