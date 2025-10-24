#include "MaterialBlinn.h"

//===============================================================================================
//Constructors
//===============================================================================================
MaterialBlinn::MaterialBlinn(std::string name)
	:Material(name, MATERIAL_NONPBR)
{
	getError("CONSTRUCTOR");
}

MaterialBlinn::MaterialBlinn(std::string name, Model& model)
	:Material(name, MATERIAL_NONPBR)
{
	getError("CONSTRUCTOR");
}

MaterialBlinn::MaterialBlinn(
	std::string name, 
	unsigned int diffuseMap, 
	unsigned int specularMap, 
	unsigned int normalMap,
	unsigned int glossinessMap,
	unsigned int emissiveMap)
	:MaterialBlinn(name)
{
	diffuse_map = diffuseMap;
	specular_map = specularMap;

	//Has normal map?
	normalMap == 0		? useNormalMap = false		: normal_map = normalMap;
	//Has glossiness map?
	glossinessMap == 0	? useGlossinessMap = false	: glossiness_map = glossinessMap;
	//Has emissive map?
	emissiveMap == 0	? useEmissiveMap = false	: emissive_map = emissiveMap;

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
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, glossiness_map);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, emissive_map);


	getError("BIND");
}
//=============================================================================================
//getError()
//=============================================================================================

void MaterialBlinn::SendToShader(const Shader& shader)
{
	shader.setInt("diffuseMap", 0);
	shader.setInt("specularMap", 1);
	shader.setInt("normalMap", 2);
	shader.setInt("glossinessMap", 3);
	shader.setInt("emissiveMap", 4);
	shader.setBool("useNormalMap", useNormalMap);
	shader.setBool("useGlossinessMap", useGlossinessMap);
	shader.setBool("useEmissiveMap", useGlossinessMap);
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