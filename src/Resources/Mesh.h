#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/Shader.h"
#include "../../src/Engine/Materials/Material.h"
#include "Texture.h"

#include <string>
#include <vector>
#include <memory>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

enum RenderType
{
	LINE,
	TRIANGLES,
	POINT
};

class Mesh
{
private:
	//id
	std::string name;

	//render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();

public:
	//mesh data
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	Material					material;

	RenderType renderType;

	//Constructor
	Mesh(
		std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices, 
		Material material);		//non-owning pointer, ResourceManager owns

	//Destructor
	~Mesh() = default;

	void Draw();
	void Draw(Shader& shader);

	//getters
	unsigned int					get_VAO() const				{ return VAO; }
	const Material&					GetMaterial() const 		{ return material; }
	const std::vector<glm::vec3>	GetVertexPositions() const;
};

