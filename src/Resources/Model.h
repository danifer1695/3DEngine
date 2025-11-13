#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <exception>

#include "Mesh.h"

class Model
{
private:
	//model data
	std::string name;
	std::vector<Mesh> meshes;
	std::string directory;

public:
	//Constructor
	Model(std::string name, std::vector<Mesh> meshes, std::string directory);

	//Drawing
	void Draw();
	void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model = glm::mat4(1.0f));

	//getters
	const std::vector<Mesh>&	GetMeshes() const;
	const std::string&			GetName()	const	{ return name; }
};

