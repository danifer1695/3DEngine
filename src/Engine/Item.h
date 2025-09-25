#pragma once

#include "GameObject.h"
#include "../Resources/Model.h"
#include "../Core/Utils.h"
#include "Materials/MaterialBlinn.h"

#include <memory>
#include <string>

class Item : public GameObject
{
private:
	//Data
	glm::vec3 IDColor = glm::vec3(1.0f);	//we can get a unique ID by adding all channels up.

	std::shared_ptr<Model> model;
	std::shared_ptr<Material> material;

public:
	//Constructor
	Item(std::string name, std::shared_ptr<Model> model, std::shared_ptr<Material> material, glm::vec3 worldPos = glm::vec3(0.0f));

	//Methods
	void	Draw(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 camPos);
	void	sendToShader(const Shader& shader);

	//Getters
	std::shared_ptr<Material>	getMaterial()		{ return material; }
	std::shared_ptr<Model>		getModel()			{ return model; }
	const glm::vec3				getIDColor() const	{ return IDColor; }
	float						GetID()				{ return IDColor.x + IDColor.y + IDColor.z; }
	AABB&						GetAABB()			{ return aabb; }
};

