#pragma once

#include "GameObject.h"
#include "../Resources/Model.h"
#include "../Resources/ResourceManager.h"
#include "../Core/Utils.h"
#include "Materials/Material.h"

#include <memory>
#include <string>

class Item : public GameObject
{
private:
	//Data
	glm::vec3 IDColor = glm::vec3(1.0f);	//we can get a unique ID by adding all channels up.

	Handle h_Model;

public:
	//Constructor
	Item(std::string name, Handle handle, glm::vec3 worldPos = glm::vec3(0.0f));

	//Methods
	void	Draw(glm::mat4 projectionMat, glm::mat4 viewMat, Shader& shader);
	void	sendToShader(const Shader& shader);
	virtual void RenderImGuiPanel() override;
	virtual void DrawModel() override;

	//Getters
	const Handle				getModelHandle() const	{ return h_Model; }
	const glm::vec3				getIDColor() const		{ return IDColor; }
	float						GetID()					{ return IDColor.x + IDColor.y + IDColor.z; }
};

