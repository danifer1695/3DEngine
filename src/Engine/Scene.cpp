#include "Scene.h"

#include<cstdlib>
#include<ctime>


//===============================================================================================
// Constructors();
//===============================================================================================

Scene::Scene(unsigned int sceneWidth, unsigned int sceneHeight)
	:sceneWidth{sceneWidth}, sceneHeight{sceneHeight}
{
	Init();
}
//===============================================================================================
// Init();
//===============================================================================================

void Scene::Init()
{
	//Initialize camera
	cameras.push_back(Camera(FLIGHT, cam_start_pos));
	activeCamera = &cameras[0];

	//Initialize skybox
	unsigned int hdriTex = ResourceManager::Get().loadHDRi(default_skybox_tex);
	skybox = std::make_unique<Skybox>(SKY_GRADIENT, hdriTex);

	//Initialize scene state variables
	projection = glm::perspective(glm::radians(activeCamera->zoom), (float)sceneWidth / (float)sceneHeight, near_plane, far_plane);

	//initialize textures
	Utils::Print("LOG - SCENE::IMPORTING_DEFAULT_TEXTURES");
	ResourceManager::Get().ImportTexture("Light_Icon",	DEFAULT, "./assets/textures/Light_Icon.png", false);
	ResourceManager::Get().ImportTexture("Item_Icon",	DEFAULT, "./assets/textures/Item_Icon.png", false);
	ResourceManager::Get().ImportTexture("Default",	DEFAULT, "./assets/textures/Default_Texture.png", false);
	ResourceManager::Get().ImportTexture("Default_Black",	DEFAULT, "./assets/textures/Default_Texture_Black.png", false);

	//initialize models
	Utils::Print("LOG - SCENE::IMPORTING_DEFAULT_MODELS");
	ModelLoader::Get().loadModel("Scene", "./assets/scenes/breakfast_room/breakfast_room.obj");
	ModelLoader::Get().loadModel("Cube", "./assets/models/cube.obj");
	
	CreateItem("Scene", ResourceManager::Get().GetModelHandle("Scene"));

	//initialize lights

	CreateLight(POINT_LIGHT);
	if(auto light = std::dynamic_pointer_cast<PointLight>(GetGameObjectCollection().back()))
	{
		light->transform.SetPosition(glm::vec3(-2.0f, 2.6f, 4.0f));
		light->SetCastShadows(false);
		light->SetIntensity(10.0f);
		light->SetColor(glm::vec3(1.0f, 0.667f, 0.31f));
	}

	CreateLight(POINT_LIGHT);
	if (auto light = std::dynamic_pointer_cast<PointLight>(GetGameObjectCollection().back()))
	{
		light->transform.SetPosition(glm::vec3(-4.1f, 2.4f, -1.95f));
		light->SetCastShadows(false);
		light->SetIntensity(3.0f);
		light->SetColor(glm::vec3(0.84f, 0.7f, 0.39f));
	}

	CreateLight(DIRECTIONAL_LIGHT);
	if (auto light = std::dynamic_pointer_cast<DirectionalLight>(GetGameObjectCollection().back()))
	{
		light->transform.SetPosition(glm::vec3(56.6f, 13.8f, 15.2f));
		light->SetCastShadows(true);
		light->SetIntensity(0.5f);
		light->SetColor(glm::vec3(0.961f, 0.914f, 0.8f));
	}

	Utils::getOpenGLError("SCENE::INIT");
}

//===============================================================================================
// UpdateScene();
//===============================================================================================
void Scene::UpdateScene()
{
	//Update scene
	projection = glm::perspective(glm::radians(activeCamera->zoom), (float)sceneWidth / (float)sceneHeight, near_plane, far_plane);;
	Utils::getOpenGLError("SCENE::UPDATE_SCENE");
}

//===============================================================================================
// CreateItem();
//===============================================================================================
std::shared_ptr<Item> Scene::CreateItem(std::string name, Handle h_model)
{
	try {
		std::shared_ptr<Item> item = std::make_shared<Item>(name, h_model);
		gameObjects.push_back(item);

		//update count
		itemCount++;

		return item;
	}
	catch (std::exception& e)
	{
		Utils::Print(e.what());
	}

	return nullptr;
}
//===============================================================================================
// CreateLight();
//===============================================================================================

std::shared_ptr<Light> Scene::CreateLight(LightType type)
{
	//We create a light shared pointer, add it to its appropriate light vector, and return it
	if (type == POINT_LIGHT)
	{
		auto pointLight = std::make_shared<PointLight>("Point Light " + std::to_string(gameObjects.size() + 1));
		gameObjects.push_back(pointLight);

		//update count
		pointLightCount++;

		return pointLight;
	}
	else if (type == DIRECTIONAL_LIGHT)
	{
		auto dirLight = std::make_shared<DirectionalLight>("Directional Light " + std::to_string(gameObjects.size() + 1));
		gameObjects.push_back(dirLight);

		//update count
		dirLightCount++;

		return dirLight;
	}
}
//=============================================================================================
//RemoveDirLight()
//=============================================================================================
  
REVIT_GO Scene::RemoveGameObject(REVIT_GO rit)
{
	if (rit != gameObjects.rend())
	{
		//Update object counts
		if (std::dynamic_pointer_cast<DirectionalLight>(*rit)) dirLightCount--;
		else if (std::dynamic_pointer_cast<PointLight>(*rit)) pointLightCount--;
		else if (std::dynamic_pointer_cast<Item>(*rit)) itemCount--;

		// erase returns a forward iterator
		auto it = gameObjects.erase((rit + 1).base());
		
		// convert forward iterator back to reverse iterator
		return std::make_reverse_iterator(it);
	}


	return rit;
}
//=============================================================================================
//GetItem()
//=============================================================================================

std::shared_ptr<Item> Scene::GetItem(const std::string& name)
{
	for (auto& obj : gameObjects)
	{
		//We are looping through all kinds of gameobjects so we want to make sure we are
		//only dealing with Item types.
		//Dynamic cast returns nullptr if not an Item class object
		if (auto item = std::dynamic_pointer_cast<Item>(obj))
		{
			if (item->GetName() == name)
				return item;
		}
	}

	//return nullptr if nothing was found
	return nullptr;
}
//=============================================================================================
//UpdateCollisions()
//=============================================================================================

void Scene::UpdateCollisions()
{
	//We will iterate through all items and update their Colliders
	for (auto& go : gameObjects)
	{
		if (go->transform.GetIsDirty())
		{
			go->GetAABB().Update(go->transform.GetModelMatrix());
		}
	}
}