#pragma once

#include "Core/GameObject.h"
#include "Model.h"
#include "Material.h"

#include <memory>
#include <string>

class Item : public GameObject
{
private:

	std::shared_ptr<Model> model;
	std::shared_ptr<Material> material;

	//private methods
	void getError(std::string location);

public:
	//Constructor
	Item(std::shared_ptr<Model> model, std::shared_ptr<Material> material, glm::vec3 worldPos = glm::vec3(0.0f));

	//Methods
	void Draw(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 camPos);
	void sendToShader(const Shader& shader);

	//Getters
	std::shared_ptr<Material>	getMaterial() { return material; }
	std::shared_ptr<Model>		getModel() { return model; }
};

