#pragma once

#include"../../vendors/ImGui/imgui.h"
#include"../../vendors/ImGui/imgui_impl_opengl3.h"
#include"../../vendors/ImGui/imgui_impl_glfw.h"

class ImGuiLayer
{
private:

	//Initialization
	void Init(GLFWwindow* window);

	//Runtime methods
	void BeginFrame();
	void EndFrame();

public:
	//Constructor
	ImGuiLayer(GLFWwindow* window);

	//Destructor
	~ImGuiLayer();

	//Runtime methods
	void Render();
};

