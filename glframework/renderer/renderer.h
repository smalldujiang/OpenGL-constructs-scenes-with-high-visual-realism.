#pragma once
#include <vector>
#include "../core.h"
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"
#include "../scene.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	//每次调用都会渲染一帧

	void render(
		Scene* scene,
		Camera* camera,
		//SpotLight* spotLight,
		DirectionLight* dirLight,
		const std::vector<PointLight*>& pointLights,
		AmbientLight* ambLight,
		unsigned int fbo = 0
	);

	void renderObject(
		Object* object,
		Camera* camera,
		//SpotLight* spotLight,
		DirectionLight* dirLight,
		const std::vector<PointLight*>& pointLights,
		AmbientLight* ambLight
	);

	void setClearColor(glm::vec3 color);

	void setDepthState(Material* material);
	void setPolygonOffsetState(Material * material);
	void setStencilState(Material* material);
	void setBlendState(Material* material);
	

private:

	Shader* pickShader(MaterialType type);


private:
	Shader* mPhongShader{ nullptr };
	Shader* mWhiteShader{ nullptr };
	Shader* mDepthShader{ nullptr };
	Shader* mScreenShader{ nullptr };
	
};