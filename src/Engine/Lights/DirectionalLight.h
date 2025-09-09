#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 target;

public:
	//Constructors
	DirectionalLight(
		const std::string name =	"Directional Light",
		const glm::vec3 position =	glm::vec3(10.0f),
		const glm::vec3 target =	glm::vec3(0.0f),
		const glm::vec3 color =		glm::vec3(1.0f),
		const bool castShadows =	true,
		const float intensity =		0.5f
	);

	//Methods
	void sendToShader(
		const Shader& shader, 
		const size_t& lightIndex, 
		const glm::mat4& view, 
		const glm::mat4& lightSpaceMatrix) const;

	//Setters
	void SetTarget(const glm::vec3& tar) { target = tar; }

	//Getters
	const glm::vec3 GetTarget() const{ return target; }
};

