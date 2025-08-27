#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Scene.h"
#include"../Core/Utils.h"
#include"../Renderer/Renderer.h"
#include"../Input/InputManager.h"
#include"../UI/ImGuiLayer.h"

#include<memory>

enum Engine_State {
	Engine_Running,
	Engine_Exit
};

class Engine
{
private:
	//Settings
	const unsigned int globalScreenWidth{ 800 }, globalScreenHeight{ 600 };

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	Engine_State engineState;

	//GLFW
	GLFWwindow* window = nullptr;

	//ImGui
	std::unique_ptr<ImGuiLayer> imGuiLayer;

	//Engine objects
	std::unique_ptr<Scene> scene = nullptr;
	std::unique_ptr<Renderer> renderer = nullptr;
	std::unique_ptr<InputManager> inputManager = nullptr;

	//Initialization
	void Init();
	void InitGLFW();
	void InitGLAD();
	void InitOpenGlState();

	//Callback functions
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		//Get pointer to this same instance
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->OnWindowResize(width, height);
	}
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
	{
		//Get pointer to this same instance
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->inputManager->MouseMoveCallback(xposIn, yposIn);
		engine->scene->SetCameraRotation(engine->inputManager->GetMouseDelta());
	}
	static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
	{
		//Get pointer to this same instance
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->scene->SetCameraScroll(y_offset);
	}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//fetch engine instance
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->inputManager->KeyCallback(key, scancode, action, mods);
	}

	//Render loop
	void Exit() { engineState = Engine_Exit; }
	
public:
	//Constructors
	Engine() { Init();}

	//Destructor
	~Engine();

	//Control
	void Run();
	void ProcessInput();

	//Callbacks
	void OnWindowResize(int width, int height);

};

