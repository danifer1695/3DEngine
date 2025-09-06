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

	//Initialization
	void Init(GLFWwindow* window);

	//Runtime methods
	//---------------
	void BeginFrame();
	void EndFrame();

	//Panels
	void RenderEntityPanel(const Scene& scene);
	void RenderAssetsPanel(const Scene& scene, unsigned int screenWidth, unsigned int screenHeight);
	void RenderScenePanel(Renderer& renderer, unsigned int screenWidth);
	void RenderViewport(const GLuint& texture, unsigned int viewPortWidth, unsigned int viewPortHeight);
	void RenderMaterialsTab(const Scene& scene);

	//Tabs
	void RenderLightTab(const Scene& scene);
	void RenderItemTab(const Scene& scene);

public:
	//Constructor
	ImGuiLayer(GLFWwindow* window);

	//Destructor
	~ImGuiLayer();

	//Runtime methods
	void Render(
		const Scene& scene, 
		Renderer& renderer, 
		unsigned int viewPortWidth, 
		unsigned int viewPortHeight,
		unsigned int screenWidth,
		unsigned int screenHeight
		);
};

