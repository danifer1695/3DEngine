#include "ResourceManager.h"

//===============================================================================================
// Get();
//===============================================================================================

ResourceManager& ResourceManager::Get()
{
	static ResourceManager instance;

	return instance;
}
//===============================================================================================
// LoadTexture();
//===============================================================================================

GLuint ResourceManager::loadTexture(char const* path, bool gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;	//this is the format of the incomming texture file
		GLenum dataFormat;		//this is the format that the texture object will have
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}
		else 
		{
			std::cerr << "Unsupported number of components: " << nrComponents << std::endl;
			stbi_image_free(data);
			return 0;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
//===============================================================================================
// LoadHDRi();
//===============================================================================================

GLuint ResourceManager::loadHDRi(char const* path)
{
	unsigned int hdrTexture;
	glGenTextures(1, &hdrTexture);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
		stbi_image_free(data);
	}

	return hdrTexture;
}
//===============================================================================================
// ImportMaterial();
//===============================================================================================

//void ResourceManager::ImportMaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath)
//{
//	//we check for duplicates
//	if (materials.find(name) == materials.end())
//		materials[name] = std::make_shared<MaterialBlinn>(name, 0);
//	else
//		std::cout << "SCENE::IMPORT_MATERIAL::WARNING: Material with name " << name << " already exists!" << std::endl;
//}
//
////overloaded method with texture map arguments
//void ResourceManager::ImportMaterialBlinn(
//	std::string name,
//	unsigned int diffuse_map,
//	unsigned int specular_map,
//	unsigned int normal_map,
//	unsigned int glossiness_map,
//	unsigned int emissive_map)
//{
//	//we check for duplicates
//	if (materials.find(name) == materials.end())
//		materials[name] = std::make_shared<MaterialBlinn>(name, diffuse_map, specular_map, normal_map, glossiness_map, emissive_map);
//	else
//		std::cout << "RESOURCE_MANAGER::IMPORT_MATERIAL::WARNING: Material with name " << name << " already exists!" << std::endl;
//}
//=============================================================================================
//ImportTexture()
//=============================================================================================

Handle ResourceManager::ImportTexture(std::string name, TextureType type, const char* path, bool gamma_correct)
{
	Handle handle = textures.Add(Texture(loadTexture(path, gamma_correct), type, path));

	//add handle to the unordered map to retrieve later
	textureIndex[name] = handle;

	return handle;
}
//=============================================================================================
//ImportModel()
//=============================================================================================

Handle ResourceManager::ImportModel(std::string name, std::vector<Mesh> meshes, std::string directory)
{
	Handle handle = models.Add(Model(name, meshes, directory));

	//add handle to the unordered map to retrieve later
	modelIndex[name] = handle;
	return handle;
}