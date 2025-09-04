#include "DirectionalLight.h"

//===============================================================================================
// Constructors();
//===============================================================================================

DirectionalLight::DirectionalLight(const std::string name, const glm::vec3 position, const glm::vec3 target, const glm::vec3 color, const bool castShadows, const float intensity)
	:Light(name, DIRECTIONAL_LIGHT, position, castShadows, color, intensity), target{ target }
{
	shadowMap = std::make_unique<ShadowMap>(Directional, 1024, 1024);
}
//=============================================================================================
//sendToShader()
//=============================================================================================

void DirectionalLight::sendToShader(const Shader& shader, const std::string& uniformName, const glm::mat4& view, const GLint& textureUnit, const glm::mat4& lightSpaceMatrix) const
{
	//we send the light's position in view space
	glm::vec3 viewPos = glm::vec3(view * glm::vec4(this->transform.getPosition(), 1.0f));

	//we send light's direction in view space
	glm::vec3 direction = glm::normalize(transform.getPosition() - target);
	glm::vec3 lightDirView = glm::mat3(view) * direction;

	shader.setBool(uniformName + ".isActive", active);
	shader.setVector3(uniformName + ".Position", viewPos);
	shader.setVector3(uniformName + ".Color", color);
	shader.setFloat(uniformName + ".Intensity", intensity);
	shader.setVector3(uniformName + ".Direction", lightDirView);

	shader.setBool(uniformName + ".CastShadow", castShadows);
	shader.setBool(uniformName + ".SoftShadow", softShadows);
	shader.setInt(uniformName + ".ShadowMap", textureUnit);

	shader.setMatrix4(uniformName + ".LightSpaceMatrix", lightSpaceMatrix);

	Utils::getOpenGLError("POINTLIGHT::SEND_TO_SHADER");
}