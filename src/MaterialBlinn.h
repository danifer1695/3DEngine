#pragma once

#include "Material.h"

class MaterialBlinn : public Material
{
private:
	//Textures
	unsigned int diffuse_map = 0;
	unsigned int specular_map = 0;

	unsigned int diffuseTexUnit = 0;
	unsigned int specularTexUnit = 1;

	float shininess = 16.0f;

public:
	//Constructors
	MaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath);
	MaterialBlinn(std::string name, 
		const char* vshaderPath, 
		const char* fshaderPath, 
		unsigned int diffuseMap,
		unsigned int specularMap);

	//Destructors

	//Getters
	virtual unsigned int getDiffuse() override { return diffuse_map; }
	virtual unsigned int getSpecular() override { return specular_map; }
	const float getShininess() { return shininess; }

	//Setters
	void setShininess(float input) { shininess = input; }

	//Methods
	virtual void bind() const override;
	virtual void useMaterial(const glm::mat4& model, 
							 const glm::mat4& projection, 
							 const glm::mat4& view, 
							 const glm::vec3& camPos) override;
	void getError(std::string) const;
};

