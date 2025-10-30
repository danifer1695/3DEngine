#include "Item.h"

//===============================================================================================
//Constructor()
//===============================================================================================

Item::Item(std::string name, Handle handle, glm::vec3 worldPos)
	:modelHandle{ handle }, GameObject(name, worldPos) 
{
	IDColor = glm::vec3(Utils::Randf(0.0f, 1.0f), Utils::Randf(0.0f, 1.0f), Utils::Randf(0.0f, 1.0f));

	InitializeAABB(ResourceManager::Get().GetModel(modelHandle));
	aabb.Update(transform.GetModelMatrix());	//Need to call update to set up worldPos min and max	

	Utils::getOpenGLError("ITEM::CONSTRUCTOR");
}

//===============================================================================================
//Draw()
//===============================================================================================
void Item::Draw(glm::mat4 projectionMat, glm::mat4 viewMat, Shader& shader)
{
	ResourceManager::Get().GetModel(modelHandle)->Draw(shader, projectionMat, viewMat, transform.GetModelMatrix());

	Utils::getOpenGLError("ITEM::DRAW::MODEL");
}
//=============================================================================================
//getError()
//=============================================================================================

void Item::sendToShader(const Shader& shader)
{
	shader.setMatrix4("model", transform.GetModelMatrix());
}