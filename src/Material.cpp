#include "Material.h"

//===============================================================================================
//Constructors
//===============================================================================================

Material::Material(std::string name)
	:name{ name } 
{
	//create shader
	this->baseShader = std::make_shared<Shader>("MATERIAL_BASE", BASE_SHADER_VS, BASE_SHADER_FS);
}

Material::Material(std::string name, const char* vshaderPath, const char* fshaderPath)
	:name{name}
{
	//create shader
	this->baseShader = std::make_shared<Shader>("MATERIAL_BASE", vshaderPath, fshaderPath);
}
//===============================================================================================
//Destructor
//===============================================================================================

Material::~Material()
{
	//clean up 
}
//===============================================================================================
//SetMatrices
//===============================================================================================

void Material::setMatrices(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view)
{
	baseShader->use();
	baseShader->setMatrix4("model", model); 
	baseShader->setMatrix4("projection", projection); 
	baseShader->setMatrix4("view", view); 

}
//===============================================================================================
//useMaterial
//===============================================================================================

void Material::useMaterial(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camPos)
{
	setMatrices(model, projection, view);
	baseShader->setMatrix3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	baseShader->setVector3("camPos", camPos);
	bind();
}
//=============================================================================================
//getError()
//=============================================================================================

void Material::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "MATERIAL::" << location << "::OpenGL error: " << err;
		if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}