#pragma once
#include "../core.h"

enum class MaterialType 
{
	PhongMaterial,
	WhiteMaterial,
	DepthMaterial,
	ScreenMaterial
};

class Material {

public:
	Material();
	~Material();

public:
	MaterialType mType;

	//��ȼ�����
	bool	mDepthTest{ true };
	GLenum  mDepthFunc{ GL_LESS };
	bool	mDepthWrite{ true };

	// polygonOffset���bool
	bool		 mPolygonOffset{ false };
	unsigned int mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	float		 mFactor{ 0.0f };
	float		 mUnit{ 0.0f };

	//ģ��������
	bool			mStencilTest{ false };
	unsigned int	mSFail{ GL_KEEP }; //ģ�����ʧ������ô��
	unsigned int	mZFail{ GL_KEEP }; //ģ�����ͨ��������ȼ��ûͨ����ô��
	unsigned int	mZPass{ GL_KEEP }; //ģ��+��Ȳ��Զ�ͨ������ô��
	unsigned int	mStencilMask{ 0xFF };//���ڿ���ģ��д��

	unsigned int	mStencilFunc{ GL_ALWAYS };
	unsigned int	mStencilRef { 0 };
	unsigned int	mStencilFuncMask{ 0xFF };

	//��ɫ���
	bool			mBlend{ false };
	unsigned int	mSFactor{ GL_SRC_ALPHA };
	unsigned int	mDFactor{ GL_ONE_MINUS_SRC_ALPHA };

	float			mOpacity{ 1.0f };

};