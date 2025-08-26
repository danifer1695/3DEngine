#pragma once
#include "Light.h"

class PointLight : public Light
{
private:
	float radius;
	float intensity;

public:
	//constructor
	PointLight(
		const glm::vec3& position_in,
		const glm::vec3& color_in,
		const bool castShadows_in,
		float radius_in = 10.0f,
		float intensity_in = 1.0f);

	//methods
	virtual void sendToShader(
		const Shader& shader, 
		const std::string& uniformName, 
		const glm::mat4& view,
		const GLint& textureUnit) const override;

	//setter
	void SetRadius(const float& rad);
	void SetIntensity(const float& i) { intensity = i; }
};

