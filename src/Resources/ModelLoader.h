#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<exception>

#include"ResourceManager.h"

class ModelLoader	//Singleton
{
private:
	//Constructor
	ModelLoader() {};

public:

	//We delete copy constructor and assignment operator
	ModelLoader(const ModelLoader&) = delete;
	ModelLoader& operator=(const ModelLoader&) = delete;

	//Get function
	static ModelLoader& Get();

	//Methods
	void					ImportModel(std::string name, const char* path);
	void					processNode(aiNode* node, const aiScene* scene, std::string name, std::vector<Mesh>& meshes, std::string path);
	Model					loadModel(std::string name, std::string path);
	Mesh					processMesh(aiMesh* mesh, const aiScene* scene, std::string path);
	std::vector<Texture>	LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType, std::string path);
};

