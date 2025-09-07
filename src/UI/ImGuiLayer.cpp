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
//RenderEntityPanel()
//=============================================================================================

void ImGuiLayer::RenderEntityPanel(Scene& scene)
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

	ImGui::Begin("Entities", NULL, windowFlags);

	if (ImGui::BeginTabBar("Entities"))
	{
		//LIGHTS TAB
		//----------
		if (ImGui::BeginTabItem("Lights"))
		{
			RenderLightTab(scene);
			ImGui::EndTabItem();
		}
		//ITEMS TAB
		//---------
		if (ImGui::BeginTabItem("Items"))
		{
			RenderItemTab(scene);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	//END OF TAB MENU

	ImGui::End();
}
//=============================================================================================
//RenderAssetsPanel()
//=============================================================================================

void ImGuiLayer::RenderAssetsPanel(const Scene& scene, unsigned int screenWidth, unsigned int screenHeight)
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

	if (ImGui::BeginTabBar("Assets"))
	{
		//MATERIALS TAB
		//----------
		if (ImGui::BeginTabItem("Materials"))
		{
			RenderMaterialsTab(scene);
			ImGui::EndTabItem();
		}
		//MODELS TAB
		//---------
		if (ImGui::BeginTabItem("Models"))
		{
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	//END OF TAB MENU

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
	//--------
	ImGui::Text("(%.1f FPS)", io.Framerate);		//Show framerate
	ImGui::Checkbox("Show demo window", &showDemo);	//Show demo window

	if (ImGui::Checkbox("SSAO active", &setSSAO))
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
//RenderMaterialsTab()
//=============================================================================================

void ImGuiLayer::RenderMaterialsTab(const Scene& scene)
{
	int i = 0;
	ImGui::NewLine();
	for (auto& material : scene.GetMaterialCollection())
	{
		ImGui::SameLine();
		ImGui::PushID(i);	//Unique ID per item
		
		//Little thumbnail displaying texture.
		//eventually to be replaced with a small render of sphere with material on
		ImTextureID imguiTexID = (ImTextureID)(intptr_t)material.second->getDiffuse();
		ImGui::ImageButton(material.second->GetName().c_str(),
			imguiTexID,
			ImVec2(64.0f, 64.0f),
			ImVec2(0, 1),
			ImVec2(1, 0));
		
		ImGui::PopID();
		i++;
	}
}
//=============================================================================================
//RenderLightTab()
//=============================================================================================

void ImGuiLayer::RenderLightTab(Scene& scene)
{
	int i = 0;
	for (auto& light : scene.GetLightCollection())
	{
		//light state variables
		glm::vec3 move = light->transform.getPosition();
		bool castShadow = light->GetCastShadows();
		bool softShadow = light->GetSoftShadows();

		ImGui::NewLine();
		ImGui::PushID(i);	//Unique ID per light

		if (ImGui::TreeNode(light->GetName().c_str()))
		{

			ImGui::Checkbox("Active", &light->active);
			ImGui::Text("Light Color:");
			ImGui::SameLine(); ImGui::ColorEdit3("Light Color", glm::value_ptr(light->color), ImGuiColorEditFlags_NoInputs);
			ImGui::InputFloat("Intensity", &light->intensity, 0.01f, 100.0f, "%.3f");
			ImGui::Text("Light Transform:");
			if (ImGui::InputFloat3("Position", glm::value_ptr(move)))
				light->transform.SetPosition(move);
			if(ImGui::Checkbox("Cast Shadows", &castShadow))
				light->ToggleCastShadows();
			if(ImGui::Checkbox("Soft Shadows", &softShadow))
				light->ToggleSoftShadows();
			ImGui::Separator();

			ImGui::TreePop();
		}
		ImGui::PopID();
		i++;
	}
	ImGui::NewLine();
	if (ImGui::Button("Add new Directional Light"))
		scene.CreateLight(DIRECTIONAL_LIGHT);
	if (ImGui::Button("Add new Point Light"))
		scene.CreateLight(POINT_LIGHT);
}
//=============================================================================================
//RenderItemTab()
//=============================================================================================

void ImGuiLayer::RenderItemTab(const Scene& scene)
{
	int i = 0;
	for (auto& item : scene.GetItemCollection())
	{
		//transformation variables
		glm::vec3 move = item.second->transform.getPosition();
		glm::vec3 scale = item.second->transform.getScale();
		glm::vec3 rotate = item.second->transform.getRotation();

		ImGui::NewLine();
		ImGui::PushID(i);	//Unique ID per light

		if (ImGui::TreeNode(item.second->GetName().c_str()))
		{
			ImGui::Text("Item Transform:");

			if (ImGui::InputFloat3("Position", glm::value_ptr(move)))
				item.second->transform.SetPosition(move);

			if (ImGui::InputFloat3("Rotation", glm::value_ptr(rotate)))
				item.second->transform.SetRotation(rotate);

			if (ImGui::InputFloat3("Scale", glm::value_ptr(scale)))
				item.second->transform.SetScale(scale);

			ImGui::Separator();

			ImGui::TreePop();
		}
		ImGui::PopID();
		i++;
	}
}
//=============================================================================================
//Render()
//=============================================================================================

void ImGuiLayer::Render(Scene& scene, Renderer& renderer, unsigned int viewPortWidth, unsigned int viewPortHeight, unsigned int screenWidth, unsigned int screenHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLuint texture = renderer.GetTexture();

	BeginFrame();

	//Add features here
	if (showDemo) ImGui::ShowDemoWindow();

	RenderEntityPanel(scene);

	RenderViewport(texture, viewPortWidth, viewPortHeight);

	RenderScenePanel(renderer, screenWidth);

	RenderAssetsPanel(scene, screenWidth, screenHeight);

	EndFrame();
}