#pragma once

#include"../../vendors/ImGui/imgui.h"
#include"../../vendors/ImGui/imgui_impl_opengl3.h"
#include"../../vendors/ImGui/imgui_impl_glfw.h"

#include"../Engine/Scene.h"

class ImGuiLayer
{
private:
	bool showDemo = false;

	//Initialization
	void Init(GLFWwindow* window);

	//Runtime methods
	void BeginFrame();
	void EndFrame();
	void RenderLightPanel(const Scene& scene);
	void RenderObjectPanel(const Scene& scene, unsigned int screenWidth, unsigned int screenHeight);
	void RenderScenePanel(const Scene& scene, unsigned int screenWidth);
	void RenderViewport(const GLuint& texture, unsigned int viewPortWidth, unsigned int viewPortHeight);

public:
	//Constructor
	ImGuiLayer(GLFWwindow* window);

	//Destructor
	~ImGuiLayer();

	//Runtime methods
	void Render(
		const Scene& scene, 
		const GLuint& texture, 
		unsigned int viewPortWidth, 
		unsigned int viewPortHeight,
		unsigned int screenWidth,
		unsigned int screenHeight
		);
};

