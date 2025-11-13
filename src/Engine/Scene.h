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
#define REVIT_GO std::vector<std::shared_ptr<GameObject>>::reverse_iterator

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
	float near_plane = 0.1f;									//frustrum near plan

	//Game Objects
	unsigned int dirLightCount = 0;
	unsigned int pointLightCount = 0;
	unsigned int itemCount = 0;
	std::vector<Camera> cameras;

	Camera* activeCamera = nullptr;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	//std::vector<std::shared_ptr<Item>> items;
	std::shared_ptr<GameObject> selectedItem = nullptr;
	std::unique_ptr<Skybox> skybox;

	//Lights
	//std::vector<std::shared_ptr<PointLight>> pointLights;
	//std::vector<std::shared_ptr<DirectionalLight>> dirLights;

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
	void SetSelectedItem(std::shared_ptr<GameObject> newSelection) { selectedItem = newSelection; }

	//Methods
	std::shared_ptr<Light> CreateLight(LightType type);
	//REVIT_DIRLIGHT		RemoveDirLight		(REVIT_DIRLIGHT rit);		//returns a reverse iterator
	//REVIT_POINTLIGHT	RemovePointLight	(REVIT_POINTLIGHT rit);		//returns a reverse iterator
	REVIT_GO			RemoveGameObject	(REVIT_GO rit);
	void				UpdateCollisions();
	std::shared_ptr<Item> CreateItem(std::string name, Handle h_model);
	
	//std::vector<std::shared_ptr<Light>> GetLightCollection() const;		//Dynamically creates a vector of all lights of all types

	//Getters
	std::shared_ptr<Item>	GetItem(const std::string& name);
	//auto&					GetItemCollection()				{ return items; }
	//auto&					GetDirLightCollection() 		{ return dirLights; }
	//auto&					GetPointLightCollection() 		{ return pointLights; }
	unsigned int			GetDirLightCount()				{ return dirLightCount; }
	unsigned int			GetPointLightCount()			{ return pointLightCount; }
	unsigned int			GetItemCount()					{ return itemCount; }
	auto&					GetGameObjectCollection()		{ return gameObjects; }
	const auto&				GetMaterialCollection() const	{ return ResourceManager::Get().GetMaterialCollection(); }
	glm::mat4				GetProjectionMatrix() const		{ return projection; }
    const Camera*			GetCamera() const				{ return activeCamera; }
	Skybox*					GetSkybox()	const				{ return skybox.get(); }
	const float				GetFarPlane() const				{ return far_plane; }
	const float				GetNearPlane() const			{ return near_plane; }
	std::shared_ptr<GameObject>	GetSelectedItem()			{ return selectedItem; }
};

