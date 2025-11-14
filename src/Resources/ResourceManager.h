#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<unordered_map>

#include"../../../stb_image.h"
#include"../Core/Utils.h"
#include"Model.h"
#include"Texture.h"
#include"ResourcePool.h"

class ResourceManager	//Singleton
{
private:
	//Constructor
	ResourceManager() {};

	//Libraries
	std::unordered_map<std::string, std::shared_ptr<Material>>	materials;	//collection of materials
	std::unordered_map<std::string, std::shared_ptr<Shader>>	shaders;	//collection of shaders

	ResourcePool<Texture> textures;
	ResourcePool<Model> models;

	std::unordered_map<std::string, Handle> textureIndex;
	std::unordered_map<std::string, Handle> modelIndex;

public:

	//We delete copy constructor and assignment operator
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//Material handling
	void ImportMaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath);
	void ImportMaterialBlinn(
		std::string name,
		unsigned int diffuse_map,
		unsigned int specular_map,
		unsigned int normal_map,
		unsigned int glossiness_map,
		unsigned int emissive_map);

	//Texture handling
	Handle		ImportTexture(std::string name, TextureType type, const char* path, bool gamma_correct);
	Handle		GetTextureHandle(std::string name)	{ return textureIndex[name]; }
	const auto	GetAllTextureHandles() const		{ return textures.GetAllHandles(); }
	Texture*	GetTexture(const Handle& handle)	{ return textures.Get(handle); }
	auto&		GetTexturePool()					{ return textures; }

	GLuint		loadHDRi(char const* path);
	GLuint		loadTexture(char const* path, bool gammaCorrection);

	//Model handling
	Handle		ImportModel(std::string name, std::vector<Mesh> meshes, std::string directory);
	Handle		GetModelHandle(std::string name)	{ return modelIndex[name]; }
	const auto	GetAllModelHandles() const			{ return models.GetAllHandles(); }
	Model*		GetModel(const Handle& handle)		{ return models.Get(handle); }
	auto&		GetModelPool()						{ return models; }

	//Getters
	static ResourceManager&		Get();

	const auto&					GetMaterialCollection() const		{ return materials; }
	std::shared_ptr<Material>	GetMaterial(std::string name)		{ return materials[name]; }
};

