#include "ImGuiLayer.h"

//=============================================================================================
//Constructor
//=============================================================================================

ImGuiLayer::ImGuiLayer(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io  = ImGui::GetIO();	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	//Enable docking

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

	//Set position of the window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 600));

	//Set window flags
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;

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
		ImGui::PushID(i);	//Unique ID per light

		if(ImGui::TreeNode(light->GetName().c_str()))
		{

			ImGui::SameLine(); ImGui::Checkbox("Active", &light->active);
			ImGui::Text("Light Color:");
			ImGui::SameLine(); ImGui::ColorEdit3("Light Color", glm::value_ptr(light->color), ImGuiColorEditFlags_NoInputs);
			ImGui::InputFloat("Intensity", &light->intensity, 0.01f, 100.0f, "%.3f");
			ImGui::Text("Light Transform:");
			if(ImGui::InputFloat3("Position", glm::value_ptr(move)))
				light->transform.SetPosition(move);
			ImGui::Checkbox("Cast Shadows", &light->castShadows);
			ImGui::Checkbox("Soft Shadows", &light->softShadows);

			ImGui::TreePop();
		}
		ImGui::PopID();
		i++;
	}
	ImGui::End();
}
//=============================================================================================
//RenderObjectPanel()
//=============================================================================================

void ImGuiLayer::RenderObjectPanel(const Scene& scene, unsigned int screenWidth, unsigned int screenHeight)
{
	int panelHeight = screenHeight - 600;

	ImGuiIO& io = ImGui::GetIO();

	//Set window parameters
	ImGui::SetNextWindowSize(ImVec2(screenWidth, panelHeight));
	ImGui::SetNextWindowPos(ImVec2(0, 600));

	//Set window flags
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Resources", NULL, windowFlags);

	//Contents

	ImGui::End();
}
//=============================================================================================
//RenderScenePanel()
//=============================================================================================

void ImGuiLayer::RenderScenePanel(Renderer& renderer, unsigned int screenWidth)
{
	int windowWidth = 800 + 400;
	bool setSSAO = renderer.GetSSAOPass().GetEnabled();

	ImGuiIO& io = ImGui::GetIO();

	//Set window parameters
	ImGui::SetNextWindowSize(ImVec2(screenWidth - windowWidth, 600));
	ImGui::SetNextWindowPos(ImVec2(windowWidth, 0));

	//Set window flags
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Scene", NULL, windowFlags);

	//Contents
	if (ImGui::Checkbox("SSAO active:", &setSSAO))
		renderer.GetSSAOPass().SetEnabled(setSSAO);

	ImGui::End();
}
//=============================================================================================
//RenderViewPort()
//=============================================================================================

void ImGuiLayer::RenderViewport(const GLuint& texture, unsigned int viewPortWidth, unsigned int viewPortHeight)
{
	ImGuiIO& io = ImGui::GetIO();

	//Set window parameters
	ImGui::SetNextWindowSize(ImVec2(viewPortWidth, viewPortHeight));
	ImGui::SetNextWindowPos(ImVec2(400, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//set padding to 0

	//Set window flags
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::Begin("ViewPort", NULL, windowFlags);

	//Draw Renderer's texture
	ImTextureID imguiTexID = (ImTextureID)(intptr_t)texture;
	ImGui::Image(imguiTexID, ImVec2((float)viewPortWidth, (float)viewPortHeight),
		ImVec2(0, 1),
		ImVec2(1, 0));

	ImGui::End();
	ImGui::PopStyleVar();
}
//=============================================================================================
//Render()
//=============================================================================================

void ImGuiLayer::Render(const Scene& scene, Renderer& renderer, unsigned int viewPortWidth, unsigned int viewPortHeight, unsigned int screenWidth, unsigned int screenHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLuint texture = renderer.GetTexture();

	BeginFrame();

	//Add features here
	if (showDemo) ImGui::ShowDemoWindow();

	RenderLightPanel(scene);

	RenderViewport(texture, viewPortWidth, viewPortHeight);

	RenderScenePanel(renderer, screenWidth);

	RenderObjectPanel(scene, screenWidth, screenHeight);

	EndFrame();
}