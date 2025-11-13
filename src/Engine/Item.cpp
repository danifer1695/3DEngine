#include "Item.h"

//===============================================================================================
//Constructor()
//===============================================================================================

Item::Item(std::string name, Handle handle, glm::vec3 worldPos)
	:h_Model{ handle }, GameObject(name, worldPos) 
{
	IDColor = glm::vec3(Utils::Randf(0.0f, 1.0f), Utils::Randf(0.0f, 1.0f), Utils::Randf(0.0f, 1.0f));

	InitializeAABB(ResourceManager::Get().GetModel(h_Model));
	aabb.Update(transform.GetModelMatrix());	//Need to call update to set up worldPos min and max	

	Utils::getOpenGLError("ITEM::CONSTRUCTOR");
}

//===============================================================================================
//Draw()
//===============================================================================================
void Item::Draw(glm::mat4 projectionMat, glm::mat4 viewMat, Shader& shader)
{
	ResourceManager::Get().GetModel(h_Model)->Draw(shader, projectionMat, viewMat, transform.GetModelMatrix());

	Utils::getOpenGLError("ITEM::DRAW::MODEL");
}
//=============================================================================================
//sendToShader()
//=============================================================================================

void Item::sendToShader(const Shader& shader)
{
	shader.setMatrix4("model", transform.GetModelMatrix());
}

//=============================================================================================
//RenderImGuiPanel()
//=============================================================================================
void Item::RenderImGuiPanel()
{
	//transformation variables
	glm::vec3 move = transform.getPosition();
	glm::vec3 scale = transform.getScale();
	glm::vec3 rotate = transform.getRotation();

	ImGui::Text(name.c_str());
	ImGui::NewLine();
	
	ImGui::Text("Item Transform:");

	if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.05f))
		transform.SetPosition(move);

	if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotate), 0.05f))
		transform.SetRotation(rotate);

	if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.05f))
		transform.SetScale(scale);
}

//=============================================================================================
//RenderImGuiPanel()
//=============================================================================================
void Item::DrawModel()
{
	ResourceManager::Get().GetModel(h_Model)->Draw();
}
