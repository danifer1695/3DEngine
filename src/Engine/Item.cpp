#include "Item.h"

//===============================================================================================
//Constructor()
//===============================================================================================

Item::Item(std::string name, std::shared_ptr<Model> model, std::shared_ptr<Material> material, glm::vec3 worldPos)
	:model{ model }, material{ material }, GameObject(name, worldPos) 
{
	IDColor = glm::vec3(Utils::Randf(0.0f, 1.0f), Utils::Randf(0.0f, 1.0f), Utils::Randf(0.0f, 1.0f));

	InitializeAABB(model);
	aabb.Update(transform.GetModelMatrix());	//Need to call update to set up worldPos min and max	

	Utils::getOpenGLError("ITEM::CONSTRUCTOR");
}

//===============================================================================================
//Draw()
//===============================================================================================
void Item::Draw(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 camPos)
{
	//set shaders, bind textures
	//material->useMaterial(transform.GetModelMatrix(), projectionMat, viewMat, camPos);
	material->bind();

	Utils::getOpenGLError("ITEM::DRAW::MATERIAL");

	//draw geometry
	model->Draw();

	Utils::getOpenGLError("ITEM::DRAW::MODEL");
}
//=============================================================================================
//getError()
//=============================================================================================

void Item::sendToShader(const Shader& shader)
{
	//naming convention should be "diffuseMap" and "specularMap";
	shader.setInt("diffuseMap", 0);
	shader.setInt("specularMap", 1);
	shader.setInt("normalMap", 2);
	shader.setBool("useNormalMap", material->GetUseNormalMap());
	shader.setMatrix4("model", transform.GetModelMatrix());
}