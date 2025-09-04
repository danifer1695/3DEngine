#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 target;

public:
	//Constructors
	DirectionalLight(
		const std::string name,
		const glm::vec3 position,
		const glm::vec3 target,
		const glm::vec3 color,
		const bool castShadows,
		const float intensity = 1.0f);

	//Methods
	void sendToShader(
		const Shader& shader, 
		const std::string& uniformName, 
		const glm::mat4& view, 
		const GLint& textureUnit, 
		const glm::mat4& lightSpaceMatrix) const;

	//Setters
	void SetTarget(const glm::vec3& tar) { target = tar; }

	//Getters
	const glm::vec3 GetTarget() const{ return target; }
};

