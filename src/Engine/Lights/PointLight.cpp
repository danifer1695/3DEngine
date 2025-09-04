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

void PointLight::sendToShader(const Shader& shader, const std::string& uniformName, const glm::mat4& view, const GLint& textureUnit) const
{
	//we send the light's position in view space
	glm::vec3 viewPos = glm::vec3(view * glm::vec4(this->transform.getPosition(), 1.0f));

	shader.setBool		(uniformName + ".isActive", active);
	shader.setVector3	(uniformName + ".Position", viewPos);
	shader.setVector3	(uniformName + ".Color", color);
	shader.setFloat		(uniformName + ".Radius", radius);
	shader.setFloat		(uniformName + ".Intensity", intensity);

	shader.setBool		(uniformName + ".CastShadow", castShadows);
	shader.setBool		(uniformName + ".SoftShadow", softShadows);
	shader.setInt		(uniformName + ".ShadowMap", textureUnit);

	Utils::getOpenGLError("POINTLIGHT::SEND_TO_SHADER");
}
//=============================================================================================
//SetRadius()
//=============================================================================================

void PointLight::SetRadius(const float& rad)
{
	radius = rad;
}
