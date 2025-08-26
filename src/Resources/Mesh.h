#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/Shader.h"

#include <string>
#include <vector>
#include <memory>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

struct Texture {
	unsigned int id;
	std::string type;	//diffuse, specular, ...
	std::string path;
};

class Mesh
{
private:
	//render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();

public:
	//mesh data
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw();
	void Draw(Shader& shader);
	void Draw(std::shared_ptr<Shader> shader);

	//getters
	unsigned int get_VAO() const { return VAO; }
};

