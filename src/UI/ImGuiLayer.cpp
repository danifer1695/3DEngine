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
//RenderLightPanel()
//=============================================================================================

void ImGuiLayer::RenderLightPanel(const Scene& scene)
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	//Enable docking

	//Set position of the window
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	//Set window flags
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoMove;

	ImGui::Begin("Lights", NULL, windowFlags);

	//Show framerate
	ImGui::Text("(%.1f FPS)", io.Framerate);
	//Show demo window
	ImGui::Checkbox("Show demo window", &showDemo);

	ImGui::SeparatorText("Lights:");
	int i = 0;
	for (auto& light : scene.GetLightCollection())
	{
		//transformation variables
		glm::vec3 move = light->transform.getPosition();

		ImGui::NewLine();

		//Whenever you create ImGui widgets in a loop, always ensure they get a unique ID. 
		//Otherwise, ImGui treats them as the same control.
		ImGui::PushID(i);

		ImGui::Text("Light %d", i);
		ImGui::SameLine(); ImGui::Checkbox("Active", &light->active);
		ImGui::Text("Light Color:");
		ImGui::SameLine(); ImGui::ColorEdit3("Light Color", glm::value_ptr(light->color), ImGuiColorEditFlags_NoInputs);
		ImGui::InputFloat("Intensity", &light->intensity, 0.01f, 100.0f, "%.3f");
		ImGui::Text("Light Transform:");
		ImGui::InputFloat3("Position", glm::value_ptr(move));
		ImGui::Checkbox("Cast Shadows", &light->castShadows);
		ImGui::Checkbox("Soft Shadows", &light->softShadows);

		ImGui::PopID();
		i++;

		//apply transformations
		//light->transform.SetPosition(move);
	}
}
//=============================================================================================
//Render()
//=============================================================================================

void ImGuiLayer::Render(const Scene& scene)
{
	BeginFrame();

	//Add features here
	if (showDemo) ImGui::ShowDemoWindow();

	RenderLightPanel(scene);

	ImGui::End();

	EndFrame();
}