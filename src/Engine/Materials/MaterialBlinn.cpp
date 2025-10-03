#include "MaterialBlinn.h"

//===============================================================================================
//Constructors
//===============================================================================================
MaterialBlinn::MaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath)
	:Material(name, MATERIAL_NONPBR, vshaderPath, fshaderPath)
{
	getError("CONSTRUCTOR");
}

MaterialBlinn::MaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath, unsigned int diffuseMap, unsigned int specularMap, unsigned int normalMap)
	:MaterialBlinn(name, vshaderPath, fshaderPath)
{
	diffuse_map = diffuseMap;
	specular_map = specularMap;

	if (normalMap == 0)
		useNormalMap = false;
	else
		normal_map = normalMap;

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
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal_map);


	getError("BIND");
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