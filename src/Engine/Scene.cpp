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

	//initialize models
	ImportModel("monkey", "./assets/models/monkey.obj");
	ImportModel("sphere", "./assets/models/sphere.obj");
	ImportModel("cube", "./assets/models/cube.obj");
	ImportModel("cone", "./assets/models/cone.obj");
	ImportModel("plane", "./assets/models/plane.obj");

	//initialize textures
	ImportTexture("grey_diff", "./assets/textures/grey_diffuse.png", false);
	ImportTexture("test01_diff", "./assets/textures/test_diffuse.png", false);
	ImportTexture("test01_spec", "./assets/textures/test_specular.png", false);
	
	//initialize materials
	ImportMaterialBlinn("blinn", "./shaders/Blinn/blinn.vs", "./shaders/Blinn/blinn.fs", textures2D["test01_diff"], textures2D["test01_spec"]);
	ImportMaterialBlinn("grey", "./shaders/Blinn/blinn.vs", "./shaders/Blinn/blinn.fs", textures2D["grey_diff"], textures2D["grey_diff"]);

	//initialize items
	items["sphere"] = std::make_unique<Item>(models["sphere"], materials["blinn"], glm::vec3(2.0f, 0.0f, 2.0f));
	items["cube"] = std::make_unique<Item>(models["cube"], materials["blinn"], glm::vec3(1.0f, 0.0f, -2.0f));
	items["cone"] = std::make_unique<Item>(models["cone"], materials["blinn"], glm::vec3(-1.0f, 0.0f, 0.0f));
	items["plane"] = std::make_unique<Item>(models["plane"], materials["grey"], glm::vec3(0.0f, 0.0f, 0.0f));

	items["plane"]->transform.Scale(glm::uvec3(100.0f, 1.0f, 100.0f));

	//initialize lights
	lights.push_back(std::make_unique<PointLight>(
		glm::vec3(3.0f, 6.0f, 8.0f),
		glm::vec3(1.0f, 0.95f, 0.8f),
		true,
		60.0f, 
		60.0f));
	lights.push_back(std::make_unique<PointLight>(
		glm::vec3(-6.0f, 2.0f, -4.0f),
		glm::vec3(1.0f, 0.25f, 0.18f),
		true,
		30.0f, 
		10.0f));
	lights.push_back(std::make_unique<PointLight>(
		glm::vec3(6.0f, 20.0f, -10.0f),
		glm::vec3(0.2f, 0.5f, 0.90f),
		false,
		80.0f, 
		90.0f));

	getError("INIT");
}
//===============================================================================================
// GetTexture();
//===============================================================================================

const unsigned int Scene::GetTexture(std::string name) const
{
	auto it = textures2D.find(name);

	//if texture not found, return 0
	if (it != textures2D.end())
		return it->second;
	else
		return 0;
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
// UpdateScene();
//===============================================================================================

void Scene::RenderSkybox() const
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	skybox->Draw(projection, activeCamera->get_view_matrix());
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

//===============================================================================================
// ImportModel();
//===============================================================================================
void Scene::ImportModel(std::string name, const char* path)
{
	//we check for duplicates
	if (models.find(name) == models.end())
		models[name] = std::make_shared<Model>(path);
	else
		std::cout << "SCENE::IMPORT_MODEL::WARNING: Element with name " << name << " already exists!" << std::endl;
}

//===============================================================================================
// ImportMaterial();
//===============================================================================================

void Scene::ImportMaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath)
{
	//we check for duplicates
	if (materials.find(name) == materials.end())
		materials[name] = std::make_shared<MaterialBlinn>(name, vshaderPath, fshaderPath);
	else
		std::cout << "SCENE::IMPORT_MATERIAL::WARNING: Material with name " << name << " already exists!" << std::endl;
}

//overloaded method with texture map arguments
void Scene::ImportMaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath, unsigned int diffuse_map, unsigned int specular_map)
{
	//we check for duplicates
	if (materials.find(name) == materials.end())
		materials[name] = std::make_shared<MaterialBlinn>(name, vshaderPath, fshaderPath, diffuse_map, specular_map);
	else
		std::cout << "SCENE::IMPORT_MATERIAL::WARNING: Material with name " << name << " already exists!" << std::endl;
}
//=============================================================================================
//ImportTexture()
//=============================================================================================

void Scene::ImportTexture(std::string name, const char* path, bool gamma_correct)
{
	//we check for duplicates
	if (textures2D.find(name) == textures2D.end())
		textures2D[name] = ResourceManager::Get().loadTexture(path, gamma_correct);
	else
		std::cout << "SCENE::IMPORT_TEXTURE::WARNING: Texture with name " << name << " already exists!" << std::endl;

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