#include "PointLight.h"

//=============================================================================================
//LightingPass()
//=============================================================================================

PointLight::PointLight(const std::string name, const glm::vec3& position_in, const glm::vec3& color_in, const bool castShadows_in, float radius_in, float intensity_in)
	:Light(name, POINT_LIGHT, position_in, castShadows_in, color_in, intensity_in), radius{ radius_in }
{
	shadowMap = std::make_unique<ShadowMap>(Point, 512, 512);
}

//=============================================================================================
//sendToShader()
//=============================================================================================

void PointLight::sendToShader(const Shader& shader, const size_t& lightIndex, const glm::mat4& view) const
{
	//we send the light's position in view space
	glm::vec3 viewPos = glm::vec3(view * glm::vec4(this->transform.getPosition(), 1.0f));

	shader.setBool		("pointLights[" + std::to_string(lightIndex) + "].isActive",	active);
	shader.setVector3	("pointLights[" + std::to_string(lightIndex) + "].Position",	viewPos);
	shader.setVector3	("pointLights[" + std::to_string(lightIndex) + "].Color",		color);
	shader.setFloat		("pointLights[" + std::to_string(lightIndex) + "].Radius",		radius);
	shader.setFloat		("pointLights[" + std::to_string(lightIndex) + "].Intensity",	intensity);

	shader.setBool		("pointLights[" + std::to_string(lightIndex) + "].CastShadow",	castShadows);
	shader.setBool		("pointLights[" + std::to_string(lightIndex) + "].SoftShadow",	softShadows);

	Utils::getOpenGLError("POINTLIGHT::SEND_TO_SHADER");
}
//=============================================================================================
//SetRadius()
//=============================================================================================

void PointLight::SetRadius(const float& rad)
{
	radius = rad;
}
