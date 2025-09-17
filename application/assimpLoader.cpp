#include "assimpLoader.h"
#include "../glframework/tools/tools.h"
#include "../glframework/material/PhongMaterial.h"
#include "../glframework/material/WhiteMaterial.h"

Object* AssimpLoader::load(const std::string& path)
{
	//拿出模型所在目录
	std::size_t lastIndex = path.find_last_of("//");
	auto rootPath = path.substr(0, lastIndex + 1);

	Object* rootNode = new Object();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR: Model Read Failed!" << std::endl;
		return nullptr;
	}

	processNode(scene->mRootNode, rootNode, scene, rootPath);

	return rootNode;

}

void AssimpLoader::processNode(aiNode* ainode, Object* parent, const aiScene* scene, const std::string& rootPath)
{
	Object* node = new Object();
	parent->addChild(node);

	glm::mat4 localMatrix = getMat4f(ainode->mTransformation);

	//位置，旋转，缩放
	glm::vec3 position, eulerAngle, scale;
	Tools::decompose(localMatrix, position, eulerAngle, scale);
	node->setPosition(position);
	node->setAngleX(eulerAngle.x);
	node->setAngleY(eulerAngle.y);
	node->setAngleZ(eulerAngle.z);
	node->setScale(scale);

	for (int i = 0; i < ainode->mNumMeshes; i++)
	{
		int meshID = ainode->mMeshes[i];
		aiMesh* aimesh = scene->mMeshes[meshID];
		auto mesh = processMesh(aimesh, scene, rootPath);
		node->addChild(mesh);
	}

	for (int i = 0; i < ainode->mNumChildren; i++)
	{
		processNode(ainode->mChildren[i], node, scene, rootPath);
	}
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 value)
{
	glm::mat4 to(
		value.a1, value.a2, value.a3, value.a4,
		value.b1, value.b2, value.b3, value.b4,
		value.c1, value.c2, value.c3, value.c4,
		value.d1, value.d2, value.d3, value.d4
	);

	return to;
}


Mesh* AssimpLoader::processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath)
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<unsigned int> indices;

	for (int i = 0; i < aimesh->mNumVertices; i++)
	{
		positions.push_back(aimesh->mVertices[i].x);
		positions.push_back(aimesh->mVertices[i].y);
		positions.push_back(aimesh->mVertices[i].z);

		if (aimesh->HasNormals())
		{
			normals.push_back(aimesh->mNormals[i].x);
			normals.push_back(aimesh->mNormals[i].y);
			normals.push_back(aimesh->mNormals[i].z);
		}
		if (aimesh->mTextureCoords[0])
		{
			uvs.push_back(aimesh->mTextureCoords[0][i].x);
			uvs.push_back(aimesh->mTextureCoords[0][i].y);
		}
		else
		{
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
		}

	}

	for (int i = 0; i < aimesh->mNumFaces; i++)
	{
		aiFace face = aimesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto geometry = new Geometry(positions, normals, uvs, indices);
	auto material = new PhongMaterial();


	if (aimesh->mMaterialIndex >= 0) {
		Texture* texture = nullptr;

		aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];

		//获取图片读取路径
		aiString aipath;
		aiMat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), aipath);

		//判断是否是嵌入fbx的图片
		const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
		if (aitexture) {
			//纹理图片是内嵌的
			unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
			uint32_t widthIn = aitexture->mWidth;
			uint32_t heightIn = aitexture->mHeight;
			texture = Texture::createTextureFromMemory(aipath.C_Str(), 0, dataIn, widthIn, heightIn);

		}
		else {
			//纹理图片在硬盘上
			std::string fullPath = rootPath + aipath.C_Str();
			texture = Texture::createTexture(fullPath, 0);
		}
		material->mDiffuse = texture;
	}
	else {
		material->mDiffuse = Texture::createTexture("assets/photos/box.png", 0);
	}
	return new Mesh(geometry, material);
}
