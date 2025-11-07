#include "PointLight.h"

//=============================================================================================
//LightingPass()
//=============================================================================================

PointLight::PointLight(const std::string name, const glm::vec3& position_in, const glm::vec3& color_in, const bool castShadows_in, float radius_in, float intensity_in)
	:Light(name, POINT_LIGHT, position_in, castShadows_in, color_in, intensity_in), radius{ radius_in }
{
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
//RenderImGuiPanel()
//=============================================================================================

void PointLight::RenderImGuiPanel()
{
	glm::vec3 move		= transform.getPosition();
	glm::vec3 color		= GetColor();
	float intensity		= GetIntensity();
	bool active			= GetActive();
	bool castShadow		= GetCastShadows();
	bool softShadow		= GetSoftShadows();
	float radius		= GetRadius();

	ImGui::NewLine();

	//Active state
	if (ImGui::Checkbox("Active", &active))
		SetActive(active);
	//Light Color
	if (ImGui::ColorEdit3("Light Color", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs))
		SetColor(color);
	//Intensity
	if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.01f, 100.0f, "%.3f"))
		SetIntensity(intensity);
	//Radius
	if (ImGui::DragFloat("Radius", &radius, 0.5f, 0.01f, 100.0f, "%.3f"))
		SetRadius(radius);

	ImGui::Text("Light Transform:");
	if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.05f))
		transform.SetPosition(move);

	if (ImGui::Checkbox("Cast Shadows", &castShadow))
		ToggleCastShadows();

	if (ImGui::Checkbox("Soft Shadows", &softShadow))
		ToggleSoftShadows();
	
}
//=============================================================================================
//SetRadius()
//=============================================================================================

void PointLight::SetRadius(const float& rad)
{
	radius = rad;
}
