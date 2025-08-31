#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<memory>

#include"../GameObject.h"
#include"../../Renderer/Shader.h"
#include"ShadowMap.h"

enum LightType
{
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOTLIGHT
};

class Light : public GameObject
{
public:
	//State
	bool active = true;

	LightType type;
	glm::vec3 color;
	float intensity;

	//Shadows
	bool castShadows;
	bool softShadows = true;

	//Shadow Maps
	std::unique_ptr<ShadowMap> shadowMap = nullptr;

	//Constructors
	Light(LightType type, 
		glm::vec3 pos, 
		bool castShadows, 
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float intensity = 1.0f);

	//Destructor
	virtual ~Light() = default;

	//methods
	glm::mat4 GetModelMatrix() const = delete;	//we wont need a model matrix for a light object

	//Getters
	ShadowMap*	GetShadowMap()	{ return shadowMap.get(); }
	const LightType	GetLightType() const { return type; }

	//Setters
	void SetIntensity(const float& i) { intensity = i; }
};

