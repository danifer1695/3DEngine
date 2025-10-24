#pragma once

#include "Material.h"
#include "../../Resources/Model.h"

class MaterialBlinn : public Material
{
private:
	//Textures
	unsigned int diffuse_map	= 0;
	unsigned int specular_map	= 0;
	unsigned int normal_map		= 0;
	unsigned int glossiness_map = 0;
	unsigned int emissive_map	= 0;

	unsigned int diffuseTexUnit		= 0;
	unsigned int specularTexUnit	= 1;
	unsigned int normalTexUnit		= 2;
	unsigned int glossinessTexUnit	= 3;
	unsigned int emissiveTexUnit	= 4;

	bool useNormalMap = true;
	bool useGlossinessMap = true;
	bool useEmissiveMap = true;

public:
	//Constructors
	MaterialBlinn(std::string name);
	MaterialBlinn(std::string name, Model& model);
	MaterialBlinn(std::string name,
		unsigned int diffuseMap,	//keep at least diffuse as mandatory so that constructors are not ambiguous
		unsigned int specularMap	= 0, 
		unsigned int normalMap		= 0,
		unsigned int glossinessMap	= 0,
		unsigned int emissiveMap	= 0);

	//Destructors

	//Getters
	virtual unsigned int	getDiffuse()		{ return diffuse_map; }
	virtual unsigned int	getSpecular()	{ return specular_map; }

	//Methods
	virtual void bind() const;
	virtual void SendToShader(const Shader& shader);
	virtual void useMaterial(const glm::mat4& model,
		const glm::mat4& projection,
		const glm::mat4& view,
		const glm::vec3& camPos) {};
	void getError(std::string) const;
};

