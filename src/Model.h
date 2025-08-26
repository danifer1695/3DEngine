#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "Mesh.h"

class Model
{
private:
	//model data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded; //to make sure the same texture isnt loaded multiple times

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

public:
	Model(const char* path);

	//Drawing
	void Draw();
	void Draw(Shader& shader);
	void Draw(std::shared_ptr<Shader> shader);
	void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model = glm::mat4(1.0f));

	//getters
	std::vector<Mesh>& get_meshes() { return meshes; }
};

