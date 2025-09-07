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

void DirectionalLight::sendToShader(const Shader& shader, const size_t& lightIndex, const glm::mat4& view, const GLint& textureUnit, const glm::mat4& lightSpaceMatrix) const
{
	//we send the light's position in view space
	glm::vec3 viewPos = glm::vec3(view * glm::vec4(this->transform.getPosition(), 1.0f));

	//we send light's direction in view space
	glm::vec3 direction = glm::normalize(transform.getPosition() - target);
	glm::vec3 lightDirView = glm::mat3(view) * direction;

	shader.setBool(		"dirLights[" + std::to_string(lightIndex) + "].isActive",			active);
	shader.setVector3(	"dirLights[" + std::to_string(lightIndex) + "].Position",			viewPos);
	shader.setVector3(	"dirLights[" + std::to_string(lightIndex) + "].Color",				color);
	shader.setFloat(	"dirLights[" + std::to_string(lightIndex) + "].Intensity",			intensity);
	shader.setVector3(	"dirLights[" + std::to_string(lightIndex) + "].Direction",			lightDirView);

	shader.setBool(		"dirLights[" + std::to_string(lightIndex) + "].CastShadow",			GetCastShadows());
	shader.setBool(		"dirLights[" + std::to_string(lightIndex) + "].SoftShadow",			GetSoftShadows());
	shader.setMatrix4(	"dirLights[" + std::to_string(lightIndex) + "].LightSpaceMatrix",	lightSpaceMatrix);

	shader.setInt(		"dirLightShadowMaps[" + std::to_string(lightIndex) + "]",	textureUnit);

	Utils::getOpenGLError("POINTLIGHT::SEND_TO_SHADER");
}