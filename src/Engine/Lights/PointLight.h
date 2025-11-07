#pragma once
#include "Light.h"

class PointLight : public Light
{
private:
	float radius;

public:
	//constructor
	PointLight(
		const std::string name =		"Point Light",
		const glm::vec3& position_in =	glm::vec3(1.0f),
		const glm::vec3& color_in =		glm::vec3(1.0f),
		const bool castShadows_in =		false,
		float radius_in =				50.0f,
		float intensity_in =			1.0f);

	//methods
	void sendToShader(
		const Shader& shader, 
		const size_t& lightIndex, 
		const glm::mat4& view) const;
	virtual void RenderImGuiPanel() override;

	//getter
	const float GetRadius() const { return radius; }

	//setter
	void SetRadius(const float& rad);
};

