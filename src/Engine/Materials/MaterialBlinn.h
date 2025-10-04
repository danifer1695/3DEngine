#pragma once

#include "Material.h"

class MaterialBlinn : public Material
{
private:
	//Textures
	unsigned int diffuse_map = 0;
	unsigned int specular_map = 0;
	unsigned int normal_map = 0;
	unsigned int glossiness_map = 0;

	unsigned int diffuseTexUnit = 0;
	unsigned int specularTexUnit = 1;
	unsigned int normalTexUnit = 2;
	unsigned int glossinessTexUnit = 3;

	bool useNormalMap = true;
	bool useGlossinessMap = true;

public:
	//Constructors
	MaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath);
	MaterialBlinn(std::string name, 
		const char* vshaderPath, 
		const char* fshaderPath, 
		unsigned int diffuseMap,
		unsigned int specularMap, 
		unsigned int normalMap = 0,
		unsigned int glossinessMap = 0);

	//Destructors

	//Getters
	virtual unsigned int	getDiffuse() override		{ return diffuse_map; }
	virtual unsigned int	getSpecular() override		{ return specular_map; }

	//Methods
	virtual void bind() const override;
	virtual void SendToShader(const Shader& shader) override;
	virtual void useMaterial(const glm::mat4& model,
		const glm::mat4& projection,
		const glm::mat4& view,
		const glm::vec3& camPos) override {};
	void getError(std::string) const;
};

