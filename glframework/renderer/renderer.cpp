#include "renderer.h"
#include <iostream>
#include "../material/PhongMaterial.h"
#include "../material/WhiteMaterial.h"
#include "../material/ScreenMaterial.h"
#include <string>

Renderer::Renderer()
{
	mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
	mDepthShader = new Shader("assets/shaders/depth.vert", "assets/shaders/depth.frag");
	mScreenShader = new Shader("assets/shaders/screen.vert", "assets/shaders/screen.frag");
}

Renderer::~Renderer()
{

}

void Renderer::render(
	Scene* scene,
	Camera* camera,
	//SpotLight* spotLight,
	DirectionLight* dirLight,
	const std::vector<PointLight*>& pointLights,
	AmbientLight* ambLight,
	unsigned int fbo
)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS); 
	glDepthMask(GL_TRUE);

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF); //保证模板缓冲可以被清理

	////默认颜色混合
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//将scene当作根节点开始递归渲染
	renderObject(scene, camera, dirLight, pointLights, ambLight);
}

//针对单个object进行渲染
void Renderer::renderObject(
	Object* object,
	Camera* camera,
	//SpotLight* spotLight,
	DirectionLight* dirLight,
	const std::vector<PointLight*>& pointLights,
	AmbientLight* ambLight
)
{
	//判断是Mesh还是Object，如果是Mesh需要渲染
	if (object->getType() == ObjectType::Mesh)
	{
		auto mesh = (Mesh*)object;
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;

		setDepthState(material);
		setPolygonOffsetState(material);
		setStencilState(material);
		setBlendState(material);

		Shader* shader = pickShader(material->mType);

		shader->use();

		switch (material->mType)
		{
		case MaterialType::PhongMaterial: {
			PhongMaterial* phongMat = (PhongMaterial*)material;

			shader->setInt("sampler", 0);
			phongMat->mDiffuse->bind();

			/*shader->setInt("specularMaskSampler", 1);*/
			/*phongMat->mSpecularMask->bind();*/

			shader->setMat4("model", mesh->getModelMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMat3("normalMatrix", normalMatrix);

			/*shader->setVec3("spotLight.position", spotLight->getPosition());
			shader->setVec3("spotLight.color", spotLight->mColor);
			shader->setVec3("spotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
			shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);*/

			//directionalLight的更新
			shader->setVec3("directionalLight.color", dirLight->mColor);
			shader->setVec3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			for (int i = 0; i < pointLights.size(); i++)
			{
				auto pointLight = pointLights[i];

				std::string baseName = "pointLights[";
				baseName.append(std::to_string(i));
				baseName.append("]");

				shader->setVec3(baseName + ".color", pointLight->mColor);
				shader->setVec3(baseName + ".position", pointLight->getPosition());
				shader->setFloat(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
				shader->setFloat(baseName + ".K1", pointLight->mK1);
				shader->setFloat(baseName + ".K2", pointLight->mK2);
				shader->setFloat(baseName + ".Kc", pointLight->mKc);
			}

			shader->setVec3("cameraPosition", camera->mPosition);
			shader->setVec3("ambientColor", ambLight->mColor);

			shader->setFloat("shiness", phongMat->mShiness);

			shader->setVec3("cameraPosition", camera->mPosition);

			shader->setFloat("opacity", phongMat->mOpacity);

		}
										break;
		case MaterialType::WhiteMaterial: {
			shader->setMat4("model", mesh->getModelMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());
		}
										break;

		case MaterialType::DepthMaterial: {
			shader->setMat4("model", mesh->getModelMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());

			shader->setFloat("near", camera->mNear); 
			shader->setFloat("far", camera->mFar);

		}
										break;

		case MaterialType::ScreenMaterial: {
			ScreenMaterial* screenMat = (ScreenMaterial*)material; 
			shader->setInt("screenTexSampler", 0);
			screenMat->mScreenTexture->bind();

		}
										break;
		default:

			break;
		}

		glBindVertexArray(geometry->getVao());

		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}

	//判断object的子节点，对每个子节点都需要调用renderObject
	auto children = object->getChildren();
	for(int i = 0; i < children.size(); i++)
	{
		renderObject(children[i], camera, dirLight, pointLights, ambLight);
	}

}


Shader* Renderer::pickShader(MaterialType type)
{
	Shader* result = nullptr;

	switch (type)
	{
	case MaterialType::PhongMaterial:
		result = mPhongShader;
		break;

	case MaterialType::WhiteMaterial:
		result = mWhiteShader;
		break;

	case MaterialType::DepthMaterial:
		result = mDepthShader;
		break;
	case MaterialType::ScreenMaterial:
		result = mScreenShader;
		break;
	default:
		std::cout << "Unknow material type to pick shader" << std::endl;
		break;
	}

	return result;

}


void Renderer::setClearColor(glm::vec3 color)
{
	glClearColor(color.r, color.g, color.b, 1.0);

}


void Renderer::setDepthState(Material* material) {
	//设置渲染状态
	if (material->mDepthTest) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(material->mDepthFunc);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (material->mDepthWrite) {
		glDepthMask(GL_TRUE);
	}
	else {
		glDepthMask(GL_FALSE);
	}
}
void Renderer::setPolygonOffsetState(Material* material) {
	//2检测polygonOffset
	if (material->mPolygonOffset) {
		glEnable(material->mPolygonOffsetType);
		glPolygonOffset(material->mFactor, material->mUnit);
	}
	else {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
}
void Renderer::setStencilState(Material* material) {
	if (material->mStencilTest) {
		glEnable(GL_STENCIL_TEST);

		glStencilOp(material->mSFail, material->mZFail, material->mZPass); 
		glStencilMask(material->mStencilMask);
		glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);

	}
	else {
		glDisable(GL_STENCIL_TEST);
	}

}

void Renderer::setBlendState(Material* material) {
	if (material->mBlend) {
		glEnable(GL_BLEND);
		glBlendFunc(material->mSFactor, material->mDFactor);
	}
	else {
		glDisable(GL_BLEND);
	}
}