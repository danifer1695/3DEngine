#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<unordered_map>

#include"../../../stb_image.h"
#include"../Engine/Materials/MaterialBlinn.h"
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
	std::unordered_map<std::string, std::shared_ptr<Model>>		models;		//collection of models
	std::unordered_map<std::string, std::shared_ptr<Texture>>	textures2D;

	ResourcePool<Texture> textures;

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
	void AddTexture(Texture tex, std::string name);	//Add texture to collection without loading
	void ImportTexture(std::string name, TextureType type, const char* path, bool gamma_correct);
	void ImportModel(std::string name, Model model);
	bool ContainsTexture(const Texture& tex);
	bool ContainsTexture(const char* path);

	//Getters
	static ResourceManager& Get();

	const auto&					GetMaterialCollection() const		{ return materials; }
	auto&						GetModelCollection()				{ return models; }
	auto&						GetTextureCollection()				{ return textures2D; }
	Texture						GetTexture(std::string name);
	Texture						GetTexture(GLuint id);				//no need to return a ref, its just an ID
	Texture						GetTextureFromPath(const char* path);		//no need to return a ref, its just an ID
	std::shared_ptr<Material>	GetMaterial(std::string name)		{ return materials[name]; }
	std::shared_ptr<Model>		GetModel(std::string name)			{ return models[name]; }
	GLuint		 loadTexture(char const* path, bool gammaCorrection);
	unsigned int loadHDRi(char const* path);
};

