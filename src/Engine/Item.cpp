#include "Item.h"

//===============================================================================================
//Constructor()
//===============================================================================================

Item::Item(std::shared_ptr<Model> model, std::shared_ptr<Material> material, glm::vec3 worldPos)
	:model{ model }, material{ material }, GameObject(worldPos) 
{

	getError("CONSTRUCTOR");
}

//===============================================================================================
//Draw()
//===============================================================================================
void Item::Draw(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 camPos)
{
	//set shaders, bind textures
	//material->useMaterial(transform.GetModelMatrix(), projectionMat, viewMat, camPos);
	material->bind();

	getError("DRAW::MATERIAL");

	//draw geometry
	model->Draw();
	getError("DRAW::MODEL");
}
//=============================================================================================
//getError()
//=============================================================================================

void Item::sendToShader(const Shader& shader)
{
	//naming convention should be "diffuseMap" and "specularMap";
	shader.setInt("diffuseMap", 0);
	shader.setInt("specularMap", 1);
	shader.setMatrix4("model", transform.GetModelMatrix());
}
//=============================================================================================
//getError()
//=============================================================================================

void Item::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "ITEM::" << location << "::OpenGL error: " << err;
		if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}