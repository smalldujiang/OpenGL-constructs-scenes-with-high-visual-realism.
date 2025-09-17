#pragma once
#include "core.h"


class Geometry {
public:
	Geometry();
	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices
	);
	~Geometry();

	static Geometry* Geometry::createBox(float size);
	static Geometry* Geometry::createSphere(float radius);
	static Geometry* Geometry::createPlane(float width, float height);
	static Geometry* Geometry::createScreen();

	GLuint getVao()const { return mVao; }
	uint32_t getIndicesCount()const { return mIndicesCount; }

private:
	GLuint mVao{ 0 };
	GLuint mPosVbo{ 0 };
	GLuint mUvVbo{ 0 };
	GLuint mNormalVbo{ 0 };
	GLuint mEbo{ 0 };

	uint32_t mIndicesCount{ 0 };
};