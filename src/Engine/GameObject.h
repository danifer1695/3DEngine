#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"../../vendors/ImGui/imgui.h"
#include"../../vendors/ImGui/imgui_impl_opengl3.h"
#include"../../vendors/ImGui/imgui_impl_glfw.h"

#include<memory>

#include"Transform.h"
#include"Collisions/AABB.h"
#include"../Core/Utils.h"
#include"../Resources/Model.h"

class GameObject
{
protected:
	AABB aabb;	//getters in child classes that will use aabbs

public:
	//Data
	std::string name;

	//State
	bool selected = false;
	Transform transform{};

	//Constructors
	GameObject(
		std::string name,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f));
	GameObject() :GameObject("default", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {};

	//UI
	virtual void RenderImGuiPanel() {};

	//Draw
	virtual void DrawModel() {};

	//Collisions
	void InitializeAABB(const Model* model);

	//Setters
	void SetName(const std::string& newName) { name = newName; }
	void SetSelected(bool set) { selected = set; }

	//Getters
	const bool			GetSelected() const		{ return selected; }
	const std::string	GetName() const			{ return name; }
	AABB&				GetAABB()				{ return aabb; }
};

