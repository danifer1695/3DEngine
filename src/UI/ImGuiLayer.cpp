#include "ImGuiLayer.h"

//=============================================================================================
//Constructor
//=============================================================================================

ImGuiLayer::ImGuiLayer(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io  = ImGui::GetIO();	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}
//=============================================================================================
//Destructor
//=============================================================================================

ImGuiLayer::~ImGuiLayer()
{
	//Terminate ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
//=============================================================================================
//Init()
//=============================================================================================

void ImGuiLayer::Init(GLFWwindow* window)
{
}
//=============================================================================================
//BeginFrame()
//=============================================================================================

void ImGuiLayer::BeginFrame()
{
	//ImGui new frame init
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
//=============================================================================================
//EndFrame()
//=============================================================================================

void ImGuiLayer::EndFrame()
{
	//ImGui render frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
//=============================================================================================
//Render()
//=============================================================================================

void ImGuiLayer::Render(const Scene& scene)
{
	ImGuiIO& io = ImGui::GetIO();
	BeginFrame();

	//Add features here
	ImGui::Begin("New Window - ImGUI");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::SeparatorText("Lights:");
	int i = 0;
	for (auto& light : scene.GetLightCollection())
	{
		//Whenever you create ImGui widgets in a loop, always ensure they get a unique ID. 
		//Otherwise, ImGui treats them as the same control.
		ImGui::PushID(i);
		ImGui::Text("Light %d", i);
		ImGui::ColorEdit3("Light Color", glm::value_ptr(light->color));
		ImGui::PopID();
		i++;
	}

	ImGui::End();

	EndFrame();
}