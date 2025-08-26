#include "MaterialBlinn.h"

//===============================================================================================
//Constructors
//===============================================================================================
MaterialBlinn::MaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath)
	:Material(name, vshaderPath, fshaderPath)
{
	baseShader->use();
	baseShader->setInt("diffuseMap", diffuseTexUnit);
	baseShader->setInt("specularMap", specularTexUnit);

	getError("CONSTRUCTOR");
}

MaterialBlinn::MaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath, unsigned int diffuseMap, unsigned int specularMap)
	:MaterialBlinn(name, vshaderPath, fshaderPath)
{
	diffuse_map = diffuseMap;
	specular_map = specularMap;
	getError("CONSTRUCTOR::WITH_TEXTURES");
}

//===============================================================================================
//bind
//===============================================================================================
void MaterialBlinn::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_map);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_map);

	getError("BIND");
}
//===============================================================================================
//useMaterial
//===============================================================================================

void MaterialBlinn::useMaterial(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& camPos)
{
	baseShader->use();

	baseShader->setMatrix4("model", model);
	baseShader->setMatrix4("projection", projection);
	baseShader->setMatrix4("view", view);
	baseShader->setVector3("viewPos", camPos);
	getError("UNIFORM_SETUP");

	bind();
}
//=============================================================================================
//getError()
//=============================================================================================

void MaterialBlinn::getError(std::string location) const
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "MATERIALBLINN::" << location << "::OpenGL error: " << err;
		if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}