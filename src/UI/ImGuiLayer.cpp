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
	ImGui_ImplOpenGL3_Init("#version 400");

	InitHandles();

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

void ImGuiLayer::InitHandles()
{
	Utils::Print("IMGUILAYER::INIT_HANDLES:: Handles initialized");
	defaultTex = ResourceManager::Get().GetTextureHandle("Item_Icon");
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
	Utils::getOpenGLError("Test1");
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	Utils::getOpenGLError("Test2");
}
//=============================================================================================
//RenderEntityPanel()
//=============================================================================================
void ImGuiLayer::Update()
{
	//Check Model dirty flags
	if (ResourceManager::Get().GetModelPool().GetIsDirty())
	{
		Utils::Print("IMGUILAYER::RENDER:: Updating Model Handles");
		modelHandles = ResourceManager::Get().GetAllModelHandles();
		ResourceManager::Get().GetModelPool().SetIsDirty(false);
	}
	//Check Texture dirty flags
	if (ResourceManager::Get().GetTexturePool().GetIsDirty())
	{
		Utils::Print("IMGUILAYER::RENDER:: Updating Texture Handles");
		textureHandles = ResourceManager::Get().GetAllTextureHandles();
		ResourceManager::Get().GetTexturePool().SetIsDirty(false);
	}
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
		//SELECTION TAB
		//----------
		if (ImGui::BeginTabItem("Selection"))
		{
			RenderSelectionTab(scene);
			ImGui::EndTabItem();
		}
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
	ImGui::SetNextWindowSize(ImVec2((float)screenWidth, (float)panelHeight));
	ImGui::SetNextWindowPos(ImVec2(0, 600));

	//Set window flags
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Resources", NULL, windowFlags);

	if (ImGui::BeginTabBar("Assets"))
	{
		//MODELS TAB
		//---------
		if (ImGui::BeginTabItem("Models"))
		{
			RenderModelsTab(scene);
			ImGui::EndTabItem();
		}
		//TEXTURES TAB
		//----------
		if (ImGui::BeginTabItem("Textures"))
		{
			RenderTexturesTab(scene);
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
	int windowWidth		= 800 + 400;
	float iconSize		= renderer.GetPostPass().GetIconSize();
	bool setSSAO		= renderer.GetSSAOPass().GetEnabled();
	bool renderIcons	= renderer.GetPostPass().GetRenderIcons();

	//select tool values
	glm::vec3 selectColor	= renderer.GetSelectColor();
	float selectThicness	= renderer.GetSelectThicness();

	ImGuiIO& io = ImGui::GetIO();

	//Set window parameters
	ImGui::SetNextWindowSize(ImVec2((float)(screenWidth - windowWidth), 600.0f));
	ImGui::SetNextWindowPos(ImVec2((float)windowWidth, 0));

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
	if(renderIDpass) renderer.SetRenderState(RENDER_ID);
	else renderer.SetRenderState(RENDER_LIGHT);
	
	//Icon rendering settings
	ImGui::Separator();
	ImGui::Text("Icons");
	if (ImGui::Checkbox("Icons visible", &renderIcons))
		renderer.GetPostPass().ToggleIcons();

	if (ImGui::DragFloat("Icon size", &iconSize, 0.005f, 0.05f, 1.0f, "%.3f"))
		renderer.GetPostPass().SetIconSize(iconSize);

	//Selection tool display settings
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
	ImGui::SetNextWindowSize(ImVec2((float)viewPortWidth, (float)viewPortHeight));
	ImGui::SetNextWindowPos(ImVec2((float)windowOffsetX, 0));
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
		Ray ray = scene.GetCamera()->GenerateRay(relativeMouseX, relativeMouseY, (float)viewPortWidth, (float)viewPortHeight, scene.GetProjectionMatrix());
		RayCaster raycaster;

		if (auto hit = raycaster.CastRay(ray, scene.GetGameObjectCollection()))
		{
			Utils::Print("Picked: " + hit->GetName());
			std::cout << "Clicked at (" << relativeMouseX << ", " << relativeMouseY << ")" << std::endl;
			
			scene.SetSelectedItem(hit);
		}
		else
		{
			scene.SetSelectedItem(nullptr);
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}
//=============================================================================================
//RenderMaterialsTab()
//=============================================================================================

void ImGuiLayer::RenderTexturesTab(Scene& scene)
{
	int i = 0;
	ImGui::NewLine();

	for (auto& handle : textureHandles)
	{
		Texture* texture = ResourceManager::Get().GetTexture(handle);
		if (!texture) continue;

		ImGui::SameLine();
		ImGui::PushID(i);	//Unique ID per item

		//Thumbnail image
		ImTextureID imguiTexID = (ImTextureID)(intptr_t)texture->ID;


		ImGui::ImageButton(texture->texPath.c_str(),
			imguiTexID,
			ImVec2(64.0f, 64.0f),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		ImGui::SetItemTooltip(texture->texPath.c_str());

		//Open popup by right clicking
		ImGui::OpenPopupOnItemClick("Texture popup", ImGuiPopupFlags_MouseButtonLeft);

		//Define popup menu
		if (ImGui::BeginPopupContextItem("Texture popup"))
		{
			if (ImGui::Selectable("Apply to selected (Not implemented)"))
			{
				//scene.CreateItem(texture->texPath, handle);
			}
			ImGui::EndPopup();
		}

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

	for (auto& handle : modelHandles)
	{
		Model* model = ResourceManager::Get().GetModel(handle);
		if (!model) continue;

		ImGui::SameLine();
		ImGui::PushID(i);	//Unique ID per item

		//Thumbnail image
		GLuint defaultTexture = ResourceManager::Get().GetTexture(defaultTex)->ID;
		ImTextureID imguiTexID = (ImTextureID)(intptr_t)defaultTexture;


		ImGui::ImageButton(model->GetName().c_str(),
			imguiTexID,
			ImVec2(64.0f, 64.0f),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		ImGui::SetItemTooltip(model->GetName().c_str());

		//Open popup by right clicking
		ImGui::OpenPopupOnItemClick("Model popup", ImGuiPopupFlags_MouseButtonLeft);

		//Define popup menu
		if (ImGui::BeginPopupContextItem("Model popup"))
		{
			if (ImGui::Selectable("Add to scene"))
				scene.CreateItem(model->GetName(), handle);
			ImGui::EndPopup();
		}

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
	for (auto rit = scene.GetGameObjectCollection().rbegin(); rit != scene.GetGameObjectCollection().rend(); )
	{
		//Dynamic cast to make sure the current GameObject is of DirectionalLight type
		//returns nullptr if its anything else
		if(auto dirlight = std::dynamic_pointer_cast<DirectionalLight>(*rit))
		{
			ImGui::NewLine();
			ImGui::PushID((int)i);	//Unique ID per light

			if (ImGui::TreeNode(rit->get()->GetName().c_str()))
			{
				rit->get()->RenderImGuiPanel();

				ImGui::NewLine();
				if (ImGui::Button("Remove"))
				{
					rit = scene.RemoveGameObject(rit);
					ImGui::TreePop();
					ImGui::PopID();
					continue;	//skip ++rit so we dont skip an element
				}

				ImGui::Separator();
				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}
		//advance iterator
		++rit;
	}
}
//=============================================================================================
//RenderPointLight()
//=============================================================================================

void ImGuiLayer::RenderPointLight(Scene& scene, size_t& i)
{
	for (auto rit = scene.GetGameObjectCollection().rbegin(); rit != scene.GetGameObjectCollection().rend(); )
	{
		//Dynamic cast to make sure the current GameObject is of PointLight type
		//returns nullptr if its anything else
		if (auto dirlight = std::dynamic_pointer_cast<PointLight>(*rit))
		{
			ImGui::NewLine();
			ImGui::PushID((int)i);	//Unique ID per light

			if (ImGui::TreeNode(rit->get()->GetName().c_str()))
			{

				rit->get()->RenderImGuiPanel();

				ImGui::NewLine();
				if (ImGui::Button("Remove"))
				{
					rit = scene.RemoveGameObject(rit);
					ImGui::TreePop();
					ImGui::PopID();
					continue;	//skip ++rit so we dont skip an element
				}

				ImGui::Separator();
				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}

		//advance iterator
		++rit;
	}
}
//=============================================================================================
//RenderItemTab()
//=============================================================================================

void ImGuiLayer::RenderItemTab(Scene& scene)
{
	int i = 0;
	for (auto& obj : scene.GetGameObjectCollection())
	{
		if(auto item = std::dynamic_pointer_cast<Item>(obj))
		{
			//transformation variables
			glm::vec3 move = item->transform.getPosition();
			glm::vec3 scale = item->transform.getScale();
			glm::vec3 rotate = item->transform.getRotation();

			ImGui::NewLine();
			ImGui::PushID(i);	//Unique ID per light

			if (ImGui::TreeNode(item->GetName().c_str()))
			{
				ImGui::Text("Item Transform:");

				if (ImGui::DragFloat3("Position", glm::value_ptr(move), 0.2f))
					item->transform.SetPosition(move);

				if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotate), 0.2f))
					item->transform.SetRotation(rotate);

				if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.2f))
					item->transform.SetScale(scale);

				ImGui::Separator();

				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}
	}
}
//=============================================================================================
//RenderSelectionTab()
//=============================================================================================

void ImGuiLayer::RenderSelectionTab(Scene& scene)
{
	//Render selected Item's UI window
	//If GetSelectedItem returns nullptr, render nothing.
	std::shared_ptr<GameObject> selection = scene.GetSelectedItem();
	if (selection != nullptr)
	{
		selection->RenderImGuiPanel();
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

	Update();

	BeginFrame();

	//Add features here
	if (showDemo) ImGui::ShowDemoWindow();

	RenderEntityPanel(scene);

	RenderViewport(scene, texture, screenWidth, screenHeight, viewPortWidth, viewPortHeight);

	RenderScenePanel(renderer, screenWidth);

	RenderAssetsPanel(scene, screenWidth, screenHeight);

	EndFrame();
}