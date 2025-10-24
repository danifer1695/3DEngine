#include "Material.h"

//===============================================================================================
//Constructors
//===============================================================================================

Material::Material(std::string name, MaterialType type)
	:name{ name }, type{type}
{
}
Material::Material(std::string name, MaterialType type, MapPackage textures, ColorPackage colors)
	:name{ name }, type{ type }, textures{textures}, colors{colors}
{
}
//===============================================================================================
//Destructor
//===============================================================================================

Material::~Material()
{
	//clean up 
}
//=============================================================================================
//bind()
//=============================================================================================

void Material::bind()
{
	if (textures.DiffuseMaps.empty())		useDiffuseMap = false;		else useDiffuseMap = true;
	if (textures.SpecularMaps.empty())		useSpecularMap = false;		else useSpecularMap = true;
	if (textures.NormalMaps.empty())		useNormalMap = false;		else useNormalMap = true;
	if (textures.EmissiveMaps.empty())		useEmissiveMap = false;		else useEmissiveMap = true;
	if (textures.GlossinessMaps.empty())	useGlossinessMap = false;	else useGlossinessMap = true;

	try {

		glActiveTexture(GL_TEXTURE0);
		if (useDiffuseMap) glBindTexture(GL_TEXTURE_2D, textures.DiffuseMaps.at(0));

		glActiveTexture(GL_TEXTURE1);
		if (useSpecularMap) glBindTexture(GL_TEXTURE_2D, textures.SpecularMaps.at(0));

		glActiveTexture(GL_TEXTURE2);
		if (useNormalMap) glBindTexture(GL_TEXTURE_2D, textures.NormalMaps.at(0));

		glActiveTexture(GL_TEXTURE3);
		if (useGlossinessMap) glBindTexture(GL_TEXTURE_2D, textures.GlossinessMaps.at(0));

		glActiveTexture(GL_TEXTURE4);
		if (useEmissiveMap) glBindTexture(GL_TEXTURE_2D, textures.EmissiveMaps.at(0));
	}
	catch (std::exception& e)
	{
		std::cout << "MATERIAL::BIND::ERROR - " << e.what() << std::endl;
	}

	Utils::getOpenGLError("BIND");
	
}
//=============================================================================================
//SendToShader()
//=============================================================================================

void Material::SendToShader(const Shader& shader)
{
	//texture maps
	shader.setInt("diffuseMap", 0);
	shader.setInt("specularMap", 1);
	shader.setInt("normalMap", 2);
	shader.setInt("glossinessMap", 3);
	shader.setInt("emissiveMap", 4);

	//non-texture colors
	shader.setVector3("diffuseColor",	colors.diffuseColor);
	shader.setFloat("specularColor",	colors.specularColor);
	shader.setVector3("normalColor",	colors.normalColor);
	shader.setVector3("emissiveColor",	colors.emissiveColor);
	shader.setFloat("glossinessColor",	colors.glossinessColor);

	//texture map checks
	shader.setBool("useDiffuseMap",		useDiffuseMap);
	shader.setBool("useSpecularMap",	useSpecularMap);
	shader.setBool("useNormalMap",		useNormalMap);
	shader.setBool("useGlossinessMap",	useGlossinessMap);
	shader.setBool("useEmissiveMap",	useEmissiveMap);
}