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
#include<unordered_map>

#include "Item.h"
#include "Camera.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "../Resources/Skybox.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/ModelLoader.h"
#include "../Core/Utils.h"

#define REVIT_DIRLIGHT std::vector<std::shared_ptr<DirectionalLight>>::reverse_iterator
#define REVIT_POINTLIGHT std::vector<std::shared_ptr<PointLight>>::reverse_iterator

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

	//Game Objects
	//std::map<std::string, std::unique_ptr<Item>> items;		//collection of items (model + material)
	std::vector<Item> items;

	std::vector<Camera> cameras;

	Camera* activeCamera = nullptr;
	Item* selectedItem = nullptr;
	std::unique_ptr<Skybox> skybox;

	//Lights
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::vector<std::shared_ptr<DirectionalLight>> dirLights;

	//Initializing
	void Init();

public:
	//Constructors
	Scene(unsigned int sceneWidth, unsigned int sceneHeight);

	//Destructors
	~Scene() {};

	//Render Loop
	void UpdateScene();

	//Camera Controls
	void SetCameraScroll(float y_offset) { activeCamera->process_mouse_scroll(y_offset); }
	void SetCameraRotation(glm::vec2 delta){activeCamera->process_mouse_movement(delta);}
	void SetCameraMovement(Camera_Movement direction, float dt) { activeCamera->process_keyboard(direction, dt); }
	void SetCameraSprint(bool isSprinting) { activeCamera->is_sprinting(isSprinting); }
	void SetSelectedItem(Item* newSelection) { selectedItem = newSelection; }

	//Methods
	void				CreateLight(LightType type);
	REVIT_DIRLIGHT		RemoveDirLight		(REVIT_DIRLIGHT rit);		//returns a reverse iterator
	REVIT_POINTLIGHT	RemovePointLight	(REVIT_POINTLIGHT rit);		//returns a reverse iterator
	void				UpdateCollisions();
	
	std::vector<std::shared_ptr<Light>> GetLightCollection() const;		//Dynamically creates a vector of all lights of all types

	//Getters
	Item*				GetItem(const std::string& name);
	auto&				GetItemCollection()				{ return items; }
	auto&				GetDirLightCollection() 		{ return dirLights; }
	auto&				GetPointLightCollection() 		{ return pointLights; }
	const auto&			GetMaterialCollection() const	{ return ResourceManager::Get().GetMaterialCollection(); }
	glm::mat4			GetProjectionMatrix() const		{ return projection; }
    const Camera*		GetCamera() const				{ return activeCamera; }
	Item*				GetSelectedItem()				{ return selectedItem; }
	Skybox*				GetSkybox()	const				{ return skybox.get(); }
	const float			GetFarPlane() const				{ return far_plane; }
	const float			GetNearPlane() const			{ return near_plane; }
};

