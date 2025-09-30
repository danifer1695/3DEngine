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

void ImGuiLayer::RenderAssetsPanel(Scene& scene, unsigned int screenWidth, unsigned int screenHeight)
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
			RenderModelsTab(scene);
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

	//select tool values
	glm::vec3 selectColor = renderer.GetSelectColor();
	float selectThicness = renderer.GetSelectThicness();

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

	if (ImGui::Checkbox("ID pass", &renderIDpass))
		renderIDpass != renderIDpass;

	//Rendering
	ImGui::Separator();
	ImGui::Text("Selection Tool");

	if(ImGui::ColorEdit3("Color", glm::value_ptr(selectColor), ImGuiColorEditFlags_NoInputs))
		renderer.SetSelectColor(selectColor);

	if(ImGui::DragFloat("Thickness", &selectThicness, 0.1f, 2.0f, 10.0f, "%.3f"))
		renderer.SetSelectThickness(selectThicness);

	ImGui::End();
}
//=============================================================================================
//RenderViewPort()
//=============================================================================================

void ImGuiLayer::RenderViewport(Scene& scene, const GLuint& texture, unsigned int screenWidth, unsigned int screenHeight, unsigned int viewPortWidth, unsigned int viewPortHeight)
{
	ImGuiIO& io = ImGui::GetIO();

	//Set window parameters
	int windowOffsetX = 400;
	ImGui::SetNextWindowSize(ImVec2(viewPortWidth, viewPortHeight));
	ImGui::SetNextWindowPos(ImVec2(windowOffsetX, 0));
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

	//Mouse Raycasting
	ImVec2 mousePos = io.MousePos;
	ImVec2 viewportPos = ImGui::GetWindowPos();

	float relativeMouseX = mousePos.x - viewportPos.x;
	float relativeMouseY = mousePos.y - viewportPos.y;

	//Check if mouse is within the viewport window
	bool coordsInRange =
		relativeMouseX >= 0 && relativeMouseX <= viewPortWidth && 
		relativeMouseY >= 0 && relativeMouseY <= viewPortHeight;

	if (coordsInRange && io.MouseClicked[0])
	{
		Ray ray = scene.GetCamera()->GenerateRay(relativeMouseX, relativeMouseY, viewPortWidth, viewPortHeight, scene.GetProjectionMatrix());
		RayCaster raycaster;

		if (Item* hit = raycaster.CastRay(ray, scene.GetItemCollection()))
		{
			Utils::Print("Picked: " + hit->GetName());
			std::cout << "Clicked at (" << relativeMouseX << ", " << relativeMouseY << ")" << std::endl;
			
			scene.SetSelectedItem(hit);
		}
		else scene.SetSelectedItem(nullptr);
	}

	ImGui::End();
	ImGui::PopStyleVar();
}
//=============================================================================================
//RenderMaterialsTab()
//=============================================================================================

void ImGuiLayer::RenderMaterialsTab(Scene& scene)
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
		ImGui::SetItemTooltip(material.second->GetName().c_str());
		
		ImGui::PopID();
		i++;
	}
}
//=============================================================================================
//RenderModelsTab()
//=============================================================================================

void ImGuiLayer::RenderModelsTab(Scene& scene)
{
	int i = 0;
	ImGui::NewLine();
	for (auto& model : scene.GetModelCollection())
	{
		ImGui::SameLine();
		ImGui::PushID(i);	//Unique ID per item

		//Little thumbnail displaying texture.
		//eventually to be replaced with a small render of sphere with material on
		ImTextureID imguiTexID = (ImTextureID)(intptr_t)scene.GetTexture("Default");
		ImGui::ImageButton(model.first.c_str(),
			imguiTexID,
			ImVec2(64.0f, 64.0f),
			ImVec2(0, 1),
			ImVec2(1, 0));
		ImGui::SetItemTooltip(model.first.c_str());

		ImGui::PopID();
		i++;
	}
}
//=============================================================================================
//RenderLightTab()
//=============================================================================================

void ImGuiLayer::RenderLightTab(Scene& scene)
{
	size_t i = 0;

	//Loop through Directional Lights using iterators
	RenderDirLight(scene, i);

	//Loop through Point Lights using iterators
	RenderPointLight(scene, i);

	ImGui::NewLine();
	if (ImGui::Button("Add new Directional Light"))
		scene.CreateLight(DIRECTIONAL_LIGHT);
	if (ImGui::Button("Add new Point Light"))
		scene.CreateLight(POINT_LIGHT);
}
//=============================================================================================
//RenderDirLight()
//=============================================================================================

void ImGuiLayer::RenderDirLight(Scene& scene, size_t& i)
{
	//Loop through Directional Lights using iterators
	for (auto rit = scene.GetDirLightCollection().rbegin(); rit != scene.GetDirLightCollection().rend(); )
	{
		//light state variables
		glm::vec3 move = rit->get()->transform.getPosition();
		bool castShadow = rit->get()->GetCastShadows();
		bool softShadow = rit->get()->GetSoftShadows();

		ImGui::NewLine();
		ImGui::PushID(i);	//Unique ID per light

		if (ImGui::TreeNode(rit->get()->GetName().c_str()))
		{

			ImGui::Checkbox("Active", &rit->get()->active);
			ImGui::ColorEdit3("Light Color", glm::value_ptr(rit->get()->color), ImGuiColorEditFlags_NoInputs);
			ImGui::DragFloat("Intensity", &rit->get()->intensity, 0.05f, 0.01f, 100.0f, "%.3f");

			ImGui::Text("Light Transform:");
			if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.2f))
				rit->get()->transform.SetPosition(move);

			if (ImGui::Checkbox("Cast Shadows", &castShadow))
				rit->get()->ToggleCastShadows();

			if (ImGui::Checkbox("Soft Shadows", &softShadow))
				rit->get()->ToggleSoftShadows();

			ImGui::NewLine();
			if (ImGui::Button("Remove"))
			{
				rit = scene.RemoveDirLight(rit);
				ImGui::TreePop();
				ImGui::PopID();
				continue;	//skip ++rit so we dont skip an element
			}

			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::PopID();
		++rit;
		i++;
	}
}
//=============================================================================================
//RenderPointLight()
//=============================================================================================

void ImGuiLayer::RenderPointLight(Scene& scene, size_t& i)
{
	for (auto rit = scene.GetPointLightCollection().rbegin(); rit != scene.GetPointLightCollection().rend(); )
	{
		//light state variables
		glm::vec3 move =	rit->get()->transform.getPosition();
		bool castShadow =	rit->get()->GetCastShadows();
		bool softShadow =	rit->get()->GetSoftShadows();
		float radius =		rit->get()->GetRadius();

		ImGui::NewLine();
		ImGui::PushID(i);	//Unique ID per light

		if (ImGui::TreeNode(rit->get()->GetName().c_str()))
		{

			ImGui::Checkbox("Active", &rit->get()->active);
			ImGui::ColorEdit3("Light Color", glm::value_ptr(rit->get()->color), ImGuiColorEditFlags_NoInputs);
			ImGui::DragFloat("Intensity", &rit->get()->intensity, 0.5f, 0.01f, 100.0f, "%.3f");

			if(ImGui::DragFloat("Radius", &radius, 0.5f, 0.01f, 100.0f, "%.3f"))
				rit->get()->SetRadius(radius);

			ImGui::Text("Light Transform:");
			if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.2f))
				rit->get()->transform.SetPosition(move);

			if (ImGui::Checkbox("Cast Shadows", &castShadow))
				rit->get()->ToggleCastShadows();

			if (ImGui::Checkbox("Soft Shadows", &softShadow))
				rit->get()->ToggleSoftShadows();

			ImGui::NewLine();
			if (ImGui::Button("Remove"))
			{
				rit = scene.RemovePointLight(rit);
				ImGui::TreePop();
				ImGui::PopID();
				continue;	//skip ++rit so we dont skip an element
			}

			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::PopID();
		++rit;
		i++;
	}
}
//=============================================================================================
//RenderItemTab()
//=============================================================================================

void ImGuiLayer::RenderItemTab(Scene& scene)
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

			if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.2f))
				item.second->transform.SetPosition(move);

			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotate), 0.2f))
				item.second->transform.SetRotation(rotate);

			if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.2f))
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
	GLuint texture = renderer.GetPostProcessed();
	if (renderIDpass) texture = renderer.GetIDPass().GetTexture();

	BeginFrame();

	//Add features here
	if (showDemo) ImGui::ShowDemoWindow();

	RenderEntityPanel(scene);

	RenderViewport(scene, texture, screenWidth, screenHeight, viewPortWidth, viewPortHeight);
	//RenderViewport(scene.GetLightCollection().at(3)->GetShadowMap()->getDepthMap(), viewPortWidth, viewPortHeight);

	RenderScenePanel(renderer, screenWidth);

	RenderAssetsPanel(scene, screenWidth, screenHeight);

	EndFrame();
}