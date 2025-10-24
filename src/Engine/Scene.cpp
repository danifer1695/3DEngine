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
	ResourceManager::Get().ImportTexture("Default",			DEFAULT,	"./assets/textures/Default_Texture_Black.png", false);

	//initialize models
	ModelLoader::Get().ImportModel("Scene", "./assets/scenes/breakfast_room/breakfast_room.obj");
	
	try {

		items["Scene"] = std::make_unique<Item>("Scene", 
			ResourceManager::Get().GetModel("Scene"));
	}
	catch (std::exception& e)
	{
		Utils::Print(e.what());
	}

	//initialize lights

	CreateLight(POINT_LIGHT);
	GetPointLightCollection().at(0)->transform.Translate(glm::vec3(0.0f, 10.0f, 0.0f));
	GetPointLightCollection().at(0)->SetCastShadows(true);
	GetPointLightCollection().at(0)->SetIntensity(50.0f);

	getError("INIT");
}
//===============================================================================================
// GetTexture();
//===============================================================================================

const GLuint Scene::GetTexture(std::string name) const
{
	return ResourceManager::Get().GetTexture(name).ID;
}

//===============================================================================================
// UpdateScene();
//===============================================================================================
void Scene::UpdateScene()
{
	//Update scene
	projection = glm::perspective(glm::radians(activeCamera->zoom), (float)sceneWidth / (float)sceneHeight, near_plane, far_plane);;
	getError("UPDATE_SCENE");
}

//===============================================================================================
// ImportModel();
//===============================================================================================
//void Scene::ImportModel(std::string name, const char* path)
//{
//	//we check for duplicates
//	if (models.find(name) == models.end())
//		models[name] = std::make_shared<Model>(path);
//	else
//		std::cout << "SCENE::IMPORT_MODEL::WARNING: Element with name " << name << " already exists!" << std::endl;
//}

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
//getError()
//=============================================================================================

void Scene::getError(std::string location)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "SCENE::" << location << "::OpenGL error: " << err;
		if (err == 1280) std::cerr << " - GL_INVALID_ENUM.";
		else if (err == 1286) std::cerr << " - Invalid Framebuffer Operation.";
		else if (err == 1282) std::cerr << " - GL_INVALID_OPERATION.";
		std::cout << std::endl;
	}
}