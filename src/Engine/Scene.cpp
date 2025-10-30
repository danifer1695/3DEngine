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
	Utils::Print("SCENE::IMPORTING_DEFAULT_TEXTURE");
	ResourceManager::Get().ImportTexture("Default",	DEFAULT, "./assets/textures/Default_Texture_Black.png", false);

	//initialize models
	ModelLoader::Get().loadModel("Scene", "./assets/scenes/breakfast_room/breakfast_room.obj");
	
	try {

		items.push_back(Item("Scene", ResourceManager::Get().GetModelHandle("Scene")));
	}
	catch (std::exception& e)
	{
		Utils::Print(e.what());
	}

	//initialize lights

	CreateLight(POINT_LIGHT);
	GetPointLightCollection().at(0)->transform.Translate(glm::vec3(0.0f, 3.0f, 0.0f));
	GetPointLightCollection().at(0)->SetCastShadows(true);
	GetPointLightCollection().at(0)->SetIntensity(10.0f);

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
// CreateLight();
//===============================================================================================

void Scene::CreateLight(LightType type)
{
	if (type == POINT_LIGHT)
	{
		pointLights.push_back(std::make_shared<PointLight>("Point Light " + std::to_string(pointLights.size() + 1)));
	}
	else if (type == DIRECTIONAL_LIGHT)
	{
		dirLights.push_back(std::make_shared<DirectionalLight>("Directional Light " + std::to_string(dirLights.size() + 1)));
	}
}
//=============================================================================================
//RemoveDirLight()
//=============================================================================================

REVIT_DIRLIGHT Scene::RemoveDirLight(REVIT_DIRLIGHT rit)
{
	if (rit != dirLights.rend())
	{
		// erase returns a forward iterator
		auto it = dirLights.erase((rit + 1).base());

		// convert forward iterator back to reverse iterator
		return std::make_reverse_iterator(it);
	}
	return rit;
}
//=============================================================================================
//RemovePointLight()
//=============================================================================================

REVIT_POINTLIGHT Scene::RemovePointLight(REVIT_POINTLIGHT rit)
{
	if (rit != pointLights.rend())
	{
		// erase returns a forward iterator
		auto it = pointLights.erase((rit + 1).base());

		// convert forward iterator back to reverse iterator
		return std::make_reverse_iterator(it);
	}
	return rit;
}
//=============================================================================================
//GetLightCollection()
//=============================================================================================

std::vector<std::shared_ptr<Light>> Scene::GetLightCollection() const
{
	std::vector<std::shared_ptr<Light>> toReturn;

	for (auto light : dirLights)
	{
		toReturn.push_back(light);
	}
	for (auto light : pointLights)
	{
		toReturn.push_back(light);
	}

	return toReturn;
}
//=============================================================================================
//GetItem()
//=============================================================================================

Item* Scene::GetItem(const std::string& name)
{
	for (Item item : items)
	{
		if (item.GetName() == name)
			return &item;
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
	for (auto& item : items)
	{
		if (item.transform.GetIsDirty())
		{
			item.GetAABB().Update(item.transform.GetModelMatrix());
		}
	}
}