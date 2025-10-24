#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"../../Renderer/Shader.h"
#include"../../Core/Utils.h"

#include<string>
#include<memory>
#include<vector>
#include<iostream>

//SHADER DIRECTORIES
#define BASE_SHADER_VS			"./shaders/baseShader.vs"
#define BASE_SHADER_FS			"./shaders/baseShader.fs"

enum MaterialType
{
	MATERIAL_NONPBR,
	MATERIAL_PBR,
	MATERIAL_DEFAULT
};

struct MapPackage
{
	std::vector<GLuint> DiffuseMaps;
	std::vector<GLuint> SpecularMaps;
	std::vector<GLuint> NormalMaps;
	std::vector<GLuint> EmissiveMaps;
	std::vector<GLuint> GlossinessMaps;
};

struct ColorPackage
{
	glm::vec3	diffuseColor	= glm::vec3(0.0f);
	float		specularColor	= 0.0f;
	glm::vec3	normalColor		= glm::vec3(0.50f, 0.50f, 1.00f);
	glm::vec3	emissiveColor	= glm::vec3(0.0f);
	float		glossinessColor = 0.1f;
};

class Material
{
protected:
	//Data
	std::string name;
	MaterialType type;

	//texture map checks
	bool useDiffuseMap;
	bool useSpecularMap;
	bool useNormalMap;
	bool useEmissiveMap;
	bool useGlossinessMap;

	MapPackage textures;
	ColorPackage colors;

public:
	//Constructor
	Material(std::string name = "default", MaterialType type = MATERIAL_NONPBR);
	Material(std::string name, MaterialType type, MapPackage textures, ColorPackage colors);

	//Destructor
	virtual ~Material();

	//Methods
	virtual void bind();
	virtual void SendToShader(const Shader& shader);

	//Getters
	const std::string		GetName() const	{ return name; }
	MapPackage				GetMapPackage() { return textures; }
};


