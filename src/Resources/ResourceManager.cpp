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

unsigned int ResourceManager::loadHDRi(char const* path)
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

void ResourceManager::ImportMaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath)
{
	//we check for duplicates
	if (materials.find(name) == materials.end())
		materials[name] = std::make_shared<MaterialBlinn>(name, 0);
	else
		std::cout << "SCENE::IMPORT_MATERIAL::WARNING: Material with name " << name << " already exists!" << std::endl;
}

//overloaded method with texture map arguments
void ResourceManager::ImportMaterialBlinn(
	std::string name,
	unsigned int diffuse_map,
	unsigned int specular_map,
	unsigned int normal_map,
	unsigned int glossiness_map,
	unsigned int emissive_map)
{
	//we check for duplicates
	if (materials.find(name) == materials.end())
		materials[name] = std::make_shared<MaterialBlinn>(name, diffuse_map, specular_map, normal_map, glossiness_map, emissive_map);
	else
		std::cout << "RESOURCE_MANAGER::IMPORT_MATERIAL::WARNING: Material with name " << name << " already exists!" << std::endl;
}
//=============================================================================================
//AddTexture()
//=============================================================================================

void ResourceManager::AddTexture(Texture tex, std::string name)
{
	//we check for duplicates
	for (auto texture : textures2D)
	{
		if (texture.second->texPath == tex.texPath)
		{
			//Don't add if a texture with the same file path already exists
			std::cout << "RESOURCE_MANAGER::ADD_TEXTURE::WARNING: Texture with name " << name << " already exists!" << std::endl;
			return;
		}
	}
	//if no duplicates are found, add to collection
	textures2D[name] = std::make_unique<Texture>(tex);
}
//=============================================================================================
//ImportTexture()
//=============================================================================================

void ResourceManager::ImportTexture(std::string name, TextureType type, const char* path, bool gamma_correct)
{
	//we check for duplicates
	for (auto tex : textures2D)
	{
		if (tex.second->texPath == path)
		{
			//Don't load if a texture with the same file path already exists
			std::cout << "SCENE::IMPORT_TEXTURE::WARNING: Texture with name " << name << " already exists!" << std::endl;
			return;
		}
	}
	//if no duplicates are found, add to collection
	textures2D[name] = std::make_unique<Texture>(loadTexture(path, gamma_correct), type, path);
}
//=============================================================================================
//ImportModel()
//=============================================================================================

void ResourceManager::ImportModel(std::string name, Model model)
{
	//we check for duplicates
	if (models.find(name) == models.end())
		models[name] = std::make_unique<Model>(model);
	else
		Utils::Print("SCENE::IMPORT_MODEL::WARNING: Model with name " + name + " already exists!");

}
//===============================================================================================
// GetTexture();
//===============================================================================================

bool ResourceManager::ContainsTexture(const Texture& tex)
{
	for (auto it : textures2D)
	{
		//strcmp returns 0 when both arguments are equal
		if (std::strcmp(it.second->texPath.c_str(), tex.texPath.c_str()) == 0)
			return true;
	}

	return false;
}

bool ResourceManager::ContainsTexture(const char* path)
{
	for (auto it : textures2D)
	{
		//strcmp returns 0 when both arguments are equal
		if (std::strcmp(it.second->texPath.c_str(), path) == 0)
			return true;
	}

	return false;
}
//===============================================================================================
// GetTexture();
//===============================================================================================

Texture ResourceManager::GetTexture(std::string name)
{
	auto it = textures2D.find(name);

	//if texture not found, return 0
	if (it != textures2D.end())
		return *it->second;
	else
		return *textures2D["Default"];
}

//search by id
Texture ResourceManager::GetTexture(GLuint id)
{
	for (auto tex : textures2D)
	{
		if (tex.second->ID == id)
		{
			return *tex.second;
		}
	}
	Utils::Print("RESOURCEMANAGER::GET_TEXTURE::ERROR: Could not find texture id");
	
	return *textures2D["Default"]; //Return default texture if no match was found
}

//search by file path
Texture ResourceManager::GetTextureFromPath(const char* path)
{
	for (auto tex : textures2D)
	{
		if (tex.second->texPath == path)
		{
			return *tex.second;
		}
	}
	std::string e = path;
	Utils::Print("RESOURCEMANAGER::GET_TEXTURE::ERROR: No texture with path: " + e);

	return *textures2D["Default"]; //Return default texture if no match was found
}

