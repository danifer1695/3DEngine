#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<vector>
#include<memory>
#include<string>
#include<map>

#include "../Model.h"
#include "../Lights/PointLight.h"
#include "../MaterialBlinn.h"
#include "../Camera.h"
#include "../Resources/Skybox.h"
#include "../ResourceManager.h"
#include "../Item.h"

//Scene class holds information on what exists, where all objects are and their properties
class Scene
{
private:
	//Directories
	const char* default_skybox_tex = "./assets/textures/moonless_golf_2k.hdr";

	//Scene state
	unsigned int sceneWidth, sceneHeight;

	//Camera state
	glm::vec3 cam_start_pos = glm::vec3(-1.5f, 2.0f, 12.0f);
	glm::mat4 projection;										//projection matrix
	float far_plane = 200.0f;									//frustrum far plane
	float near_plane = 0.1f;									//frustrum near plane

	//Asset libraries
	std::map<std::string, std::shared_ptr<Model>> models;		//collection of models
	std::map<std::string, std::shared_ptr<Material>> materials;	//collection of materials
	std::map<std::string, unsigned int> textures2D;

	//Game Objects
	std::map<std::string, std::unique_ptr<Item>> items;			//collection of items (model + material)
	std::vector<std::unique_ptr<Light>> lights;		//collection of lights
	std::vector<Camera> cameras;

	Camera* activeCamera = nullptr;
	std::unique_ptr<Skybox> skybox;

	//Initializing
	void Init();

	//Utilities
	void ImportModel(std::string name, const char* path);
	void ImportMaterialBlinn(std::string name, const char* vshaderPath, const char* fshaderPath);
	void ImportMaterialBlinn(std::string name, 
		const char* vshaderPath, 
		const char* fshaderPath, 
		unsigned int diffuse_map, 
		unsigned int specular_map);
	void ImportTexture(std::string name, const char* path, bool gamma_correct);
	void getError(std::string location);

public:
	//Constructors
	Scene(unsigned int sceneWidth, unsigned int sceneHeight);

	//Destructors
	~Scene() {};

	//Render Loop
	void UpdateScene();
	void RenderSkybox() const;

	//Camera Controls
	void SetCameraScroll(float y_offset) { activeCamera->process_mouse_scroll(y_offset); }
	void SetCameraRotation(glm::vec2 delta){activeCamera->process_mouse_movement(delta);}
	void SetCameraMovement(Camera_Movement direction, float dt) { activeCamera->process_keyboard(direction, dt); }
	void SetCameraSprint(bool isSprinting) { activeCamera->is_sprinting(isSprinting); }

	//Getters
	const auto&			GetItemCollection() const		{ return items; }
	const auto&			GetLightCollection() const		{ return lights; }
	glm::mat4			GetProjectionMatrix() const		{ return projection; }
    const Camera*		GetCamera() const				{ return activeCamera; }
	const Skybox*		GetSkybox() const				{ return skybox.get(); }
	const float			GetFarPlane() const				{ return far_plane; }
	const float			GetNearPlane() const			{ return near_plane; }

	const unsigned int	GetTexture(std::string name) const;
};

