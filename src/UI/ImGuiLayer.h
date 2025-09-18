#pragma once

#include"../../vendors/ImGui/imgui.h"
#include"../../vendors/ImGui/imgui_impl_opengl3.h"
#include"../../vendors/ImGui/imgui_impl_glfw.h"

#include"../Engine/Scene.h"
#include"../Renderer/Renderer.h"

class ImGuiLayer
{
private:
	bool showDemo = false;
	bool renderIDpass = false;

	//Initialization
	void Init(GLFWwindow* window);

	//Runtime methods
	//---------------
	void BeginFrame();
	void EndFrame();

	//Panels
	void RenderEntityPanel(Scene& scene);
	void RenderAssetsPanel(Scene& scene, unsigned int screenWidth, unsigned int screenHeight);
	void RenderScenePanel(Renderer& renderer, unsigned int screenWidth);
	void RenderViewport(const GLuint& texture, unsigned int viewPortWidth, unsigned int viewPortHeight);

	//Tabs
	void RenderLightTab(Scene& scene);
	void RenderItemTab(Scene& scene);
	void RenderMaterialsTab(Scene& scene);
	void RenderModelsTab(Scene& scene);

	//Elements
	void RenderDirLight(Scene& scene, size_t& index);
	void RenderPointLight(Scene& scene, size_t& index);

public:
	//Constructor
	ImGuiLayer(GLFWwindow* window);

	//Destructor
	~ImGuiLayer();

	//Runtime methods
	void Render(
		Scene& scene, 
		Renderer& renderer, 
		unsigned int viewPortWidth, 
		unsigned int viewPortHeight,
		unsigned int screenWidth,
		unsigned int screenHeight
		);
};

