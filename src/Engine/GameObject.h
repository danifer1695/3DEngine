#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<memory>

#include"Transform.h"
#include"../Core/Utils.h"

class GameObject
{
public:
	//Data
	std::string name;

	//State
	Transform transform{};

	//Constructors
	GameObject(
		std::string name,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)) 
		:name{name}
	{
		transform.SetPosition(position);
		transform.SetRotation(rotation);
		transform.SetScale(scale);
	};
	GameObject() :GameObject("default", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {};

	//Setters
	void SetName(const std::string& newName) { name = newName; }

	//Getters
	const std::string GetName() const { return name; }
};

