#pragma once
#include "Light.h"

class PointLight : public Light
{
private:
	float radius;

public:
	//constructor
	PointLight(
		const std::string name,
		const glm::vec3& position_in,
		const glm::vec3& color_in,
		const bool castShadows_in,
		float radius_in = 10.0f,
		float intensity_in = 1.0f);

	//methods
	void sendToShader(
		const Shader& shader, 
		const std::string& uniformName, 
		const glm::mat4& view,
		const GLint& textureUnit) const;

	//setter
	void SetRadius(const float& rad);
};

