#include "DirectionalLight.h"

//===============================================================================================
// Constructors();
//===============================================================================================

DirectionalLight::DirectionalLight(const std::string name, const glm::vec3 position, const glm::vec3 target, const glm::vec3 color, const bool castShadows, const float intensity)
	:Light(name, DIRECTIONAL_LIGHT, position, castShadows, color, intensity), target{ target }
{
}
//=============================================================================================
//sendToShader()
//=============================================================================================

void DirectionalLight::sendToShader(const Shader& shader, const size_t& lightIndex, const glm::mat4& view, const glm::mat4& lightSpaceMatrix) const
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

	Utils::getOpenGLError("POINTLIGHT::SEND_TO_SHADER");
}
//=============================================================================================
//RenderImGuiPanel()
//=============================================================================================

void DirectionalLight::RenderImGuiPanel()
{
	//light state variables
	glm::vec3 move		= transform.getPosition();
	glm::vec3 color		= GetColor();
	float intensity		= GetIntensity();
	bool active			= GetActive();
	bool castShadow		= GetCastShadows();
	bool softShadow		= GetSoftShadows();

	//Active state
	if (ImGui::Checkbox("Active", &active))
		SetActive(active);
	//Light Color
	if (ImGui::ColorEdit3("Light Color", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs))
		SetColor(color);
	//Intensity
	if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.01f, 100.0f, "%.3f"))
		SetIntensity(intensity);

	ImGui::Text("Light Transform:");
	if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.2f))
		transform.SetPosition(move);

	if (ImGui::Checkbox("Cast Shadows", &castShadow))
		ToggleCastShadows();

	if (ImGui::Checkbox("Soft Shadows", &softShadow))
		ToggleSoftShadows();
}
